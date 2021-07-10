#include "tmbl/config.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <cstddef>

namespace tmbl {

class registers;

ppu::ppu(registers &regs_, cartridge &cart_, interrupts &intr_)
    : m_regs(regs_), m_cart(cart_), m_intr(intr_), color_gameboy_support(cart_.cgbSupport()),
      STAT(regs_.getAt(0xFF41), /*ly*/ regs_.getAt(0xFF44), /*lyc*/ regs_.getAt(0xFF45)),
      LCDC(regs_.getAt(0xFF40), cart_.cgbSupport()),      // lcd controller
      SCY(regs_.getAt(0xFF42)), SCX(regs_.getAt(0xFF43)), // screen (viewport) scroll
      LY(regs_.getAt(0xFF44)), LYC(regs_.getAt(0xFF45)),
      BGP(regs_.getAt(0xFF47)),                             // background palette
      OBP0(regs_.getAt(0xFF48)), OBP1(regs_.getAt(0xFF49)), // object palettes
      WY(regs_.getAt(0xFF4A)), WX(regs_.getAt(0xFF4B)),     // window position
      VBK(regs_.getAt(0xFF4F)),                             // color gameboy vram bank control
      BCPS(regs_.getAt(0xFF68)), BCPD(regs_.getAt(0xFF69)), // cgb background palettes
      OCPS(regs_.getAt(0xFF6A)), OCPD(regs_.getAt(0xFF6B))  // cgb object palettes
{

  if (m_cart.cgbSupport()) {
    m_vram.resize(16_KB);
  } else {
    m_vram.resize(8_KB);
  }
}

void ppu::update(const std::function<void(tmbl::ppu::frame &framebuffer)> &drawCallback) {
  if (LCDC.lcdControllerStatus() == on) {

    switch (STAT.modeFlag()) {
      case statMode::SEARCHING_OAM: //////////////////////////////////////// mode 2
      {
        m_intr.LCDC_Status_IRQ = STAT.matchSearchOAM();

        scanline();

        m_clock.wait(oamCycles);
        STAT.modeFlag(statMode::TRANSFERING_DATA_TO_LCD);
      } break;

      case statMode::TRANSFERING_DATA_TO_LCD: ////////////////////////////// mode 3
        drawCallback(m_frame);

        m_clock.wait(vramCycles);
        STAT.modeFlag(statMode::HORIZONTAL_BLANKING);
        break;

      case statMode::HORIZONTAL_BLANKING: ////////////////////////////////// mode 0
        ++LY;
        m_intr.LCDC_Status_IRQ = STAT.matchHblank();

        m_clock.wait(hblankCycles);
        // drawing scanline has finished,
        // now if not vertical blanking, scan other line

        if (STAT.matchFlag()) { // LY == LYC
          m_intr.LCDC_Status_IRQ = STAT.matchCoincidence();
        }

        if (LY == screenHeight) {
          STAT.modeFlag(statMode::VERTICAL_BLANKING);
        } else {
          STAT.modeFlag(statMode::SEARCHING_OAM);
        }
        break;

      case statMode::VERTICAL_BLANKING: //////////////////////////////////// mode 1
        m_intr.VBlank_IRQ = STAT.matchVblank();

        for (uint8 i = 0; i < 10; ++i) { // 10 scanlines of vertical blank
          ++LY;
          m_clock.wait(scanlineCycles);
        }

        if (LY == screenVBlankHeight) {
          LY = 0;
        }

        STAT.modeFlag(statMode::SEARCHING_OAM);
        break;
    }
  } else { // lcdc off
    LY = 0;
    STAT.modeFlag(statMode::VERTICAL_BLANKING);
  }
}

byte ppu::readVRAM(const std::size_t index) const noexcept {
  const std::size_t effective_index = 8_KB * vbk() + index; // 8KB is VRAM bank size
  return m_vram.at(effective_index);
}

void ppu::writeVRAM(const std::size_t index, const byte val) noexcept {
  const std::size_t effective_index = 8_KB * vbk() + index;
  m_vram.at(effective_index) = val;
}

byte ppu::readOAM(const std::size_t index) const noexcept { return m_oam.at(index); }

void ppu::writeOAM(const std::size_t index, const byte val) noexcept { m_oam.at(index) = val; }

statMode ppu::status() const noexcept { return STAT.modeFlag(); }

void ppu::fetchBackground() noexcept {
  const uint16 y = SCY + LY;
  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    if (y >= 0 && y < screenHeight && SCX + dx < screenWidth) {
      const uint16 x = SCX + dx;
      const uint16 tileIndex = (x / tileWidth) + ((y / tileHeight) * 32);

      const auto [tilemap, _] = LCDC.bgTilemapSelect();
      const byte value = readVRAM(tilemap + tileIndex);

      const auto [tileptrBase, isSigned] = LCDC.tilesetBasePtr();
      const uint16 tileptr = isSigned ? (tileptrBase + ((value - 128) * tileSize))
                                      : (tileptrBase + (value * tileSize));

      const uint8 tileRowNumber = y % tileHeight;
      const byte tilelineLowByte =
          readVRAM(tileptr + (tileRowNumber * 2)); // 2 is # of bytes in tilerow
      const byte tilelineHighByte = readVRAM(tileptr + (tileRowNumber * 2) + 1);

      // scan bits of tileline bytes from left to right
      const uint8 tileColumnNumber = x % tileWidth;
      const uint8 shiftNthBitToTest = 7 - tileColumnNumber;
      const bool loBit = (tilelineLowByte >> shiftNthBitToTest) & 0b0000'0001;
      const bool hiBit = (tilelineHighByte >> shiftNthBitToTest) & 0b0000'0001;

      const uint8 paletteIndex = (hiBit << 1) | loBit;

      if (color_gameboy_support) {
        // implement color gameboy palette things
      } else {
        m_frame.at(LY).at(dx) = dmg_palette[BGP[paletteIndex]];
      }
    }
  }
}

void ppu::fetchWindow() noexcept {
  const uint16 y = LY - WY;

  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    if (dx >= WX - 7) {
      const uint16 x = dx - WX + 7; // 7 is window offset

      const uint16 tileIndex = (x / tileWidth) + ((y / tileHeight) * 32);

      const auto [tilemap, _] = LCDC.winTilemapSelect();
      const auto [tileptrBase, isSigned] = LCDC.tilesetBasePtr();
      const byte value = readVRAM(tilemap + tileIndex);

      // clang-format off
      const uint16 tileptr = isSigned 
	                     ? (tileptrBase + ((value - 128) * tileSize))
	                     : (tileptrBase + (value * tileSize));
      // clang-format on

      const uint8 tileRowNumber = y % tileHeight;
      const byte tilelineLowByte = readVRAM(tileptr + (tileRowNumber * 2));
      const byte tilelineHighByte = readVRAM(tileptr + (tileRowNumber * 2) + 1);

      const uint8 tileColumnNumber = x % tileWidth;
      const uint8 shiftNthBitToTest = 7 - tileColumnNumber;
      const bool loBit = (tilelineLowByte >> shiftNthBitToTest) & 0b0000'0001;
      const bool hiBit = (tilelineHighByte >> shiftNthBitToTest) & 0b0000'0001;

      const uint8 paletteIndex = (hiBit << 1) | loBit;

      if (color_gameboy_support) {
        // implement color gameboy palette things
      } else {
        m_frame.at(LY).at(dx) = dmg_palette[BGP[paletteIndex]];
      }
    }
  }
}

void ppu::fetchSprite() noexcept {
  for (std::size_t oamIndex = 0; oamIndex < m_oam.size(); oamIndex += 4) {
    const uint8 spriteHeight = LCDC.spriteHeight();

    const byte yPos = readOAM(oamIndex) - 16;
    const byte xPos = readOAM(oamIndex + 1) - 8;

    if ((yPos <= LY) && ((yPos + spriteHeight) > LY) && (xPos >= 0) && (xPos < screenWidth)) {
      byte tileNumber = readOAM(oamIndex + 2);
      const byte attribute = readOAM(oamIndex + 3);

      const flag bgHasPriority = attribute & 0b1000'0000;
      const flag yFlip = attribute & 0b0100'0000;
      const flag xFlip = attribute & 0b0010'0000;
      const uint8 dmgPalette = attribute & 0b0001'0000;

      uint8 tileRow = (LY - yPos) % spriteHeight;
      if (yFlip) {
        tileRow = (spriteHeight - 1) - tileRow;
      }

      if (spriteHeight == 16) {
        if (tileRow >= 8) {
          tileNumber |= 0b0000'0001;
          tileRow -= tileHeight;
        } else {
          tileNumber &= 0b1111'1110;
        }
      }

      const byte tilelineLowByte = m_vram.at((tileNumber * tileSize) + (tileRow * 2));
      const byte tilelineHighByte = m_vram.at((tileNumber * tileSize) + (tileRow * 2) + 1);

      if (color_gameboy_support) {
        //
      } else {
        for (uint8 tileLineColumn = 0; tileLineColumn < tileWidth; ++tileLineColumn) {

          if (tileLineColumn + xPos < screenWidth) {
            bool loBit, hiBit;
            if (xFlip) {
              loBit = (tilelineLowByte >> tileLineColumn) & 0b1;
              hiBit = (tilelineHighByte >> tileLineColumn) & 0b1;
            } else {
              loBit = (tilelineLowByte >> (7 - tileLineColumn)) & 0b1;
              hiBit = (tilelineHighByte >> (7 - tileLineColumn)) & 0b1;
            }

            const std::size_t paletteIndex = (hiBit << 1) | loBit;

            if (dmgPalette == 1) {
              m_frame.at(LY).at(tileLineColumn + xPos) = dmg_palette[OBP1[paletteIndex]];
            } else {
              m_frame.at(LY).at(tileLineColumn + xPos) = dmg_palette[OBP0[paletteIndex]];
            }
          }
        }
      }
    }
  }
} // :(

void ppu::scanline() noexcept {
  if (LCDC.bgDisplayStatus() == on) {
    fetchBackground();
  }

  if (LY >= WY && LCDC.winDisplayStatus() == on) {
    fetchWindow();
  }

  if (LCDC.objDisplayStatus() == on) {
    fetchSprite();
  }
}

uint8 ppu::vbk() const noexcept { return color_gameboy_support ? (VBK & 0b0000'0001) : 0; }
}
