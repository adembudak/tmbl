#include "tmbl/config.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <cstddef> // for std::size_t

namespace tmbl {

ppu::ppu(cartridge &cart_, registers &regs_, interrupts &intr_)
    : m_cart(cart_), m_regs(regs_), m_intr(intr_), color_gameboy_support(cart_.cgbSupport()),
      STAT(regs_.getAt(0xFF41), /*ly*/ regs_.getAt(0xFF44), /*lyc*/ regs_.getAt(0xFF45)),
      LCDC(regs_.getAt(0xFF40), cart_.cgbSupport()),      // lcd controller
      SCY(regs_.getAt(0xFF42)), SCX(regs_.getAt(0xFF43)), // screen (viewport) scroll
      LY(regs_.getAt(0xFF44)), LYC(regs_.getAt(0xFF45)),
      BGP(regs_.getAt(0xFF47)),                             // background palette
      OBP0(regs_.getAt(0xFF48)), OBP1(regs_.getAt(0xFF49)), // object palettes
      WY(regs_.getAt(0xFF4A)), WX(regs_.getAt(0xFF4B)),     // window position
      VBK(regs_.getAt(0xFF4F)),                             // color gameboy vram bank control
      BCPS{regs_.getAt(0xFF68)}, BCPD{regs_.getAt(0xFF69)}, // cgb background palettes
      OCPS(regs_.getAt(0xFF6A)), OCPD(regs_.getAt(0xFF6B))  // cgb object palettes
{}

void ppu::update(const std::function<void(tmbl::ppu::frame &framebuffer)> &drawCallback) {

  if (LCDC.lcdControllerStatus() == on) {

    switch (STAT.modeFlag()) {
      case statMode::SEARCHING_OAM: //////////////////////////////////////// mode 2
        m_intr.LCDC_Status_IRQ = STAT.matchSearchOAM();

        scanline();

        m_clock.wait(oamCycles);
        STAT.modeFlag(statMode::TRANSFERING_DATA_TO_LCD);
        break;

      case statMode::TRANSFERING_DATA_TO_LCD: ////////////////////////////// mode 3
        drawCallback(m_frame);

        // hdma here
        m_clock.wait(vramCycles);
        STAT.modeFlag(statMode::HORIZONTAL_BLANKING);
        break;

      case statMode::HORIZONTAL_BLANKING: ////////////////////////////////// mode 0
        updateLY();
        m_intr.LCDC_Status_IRQ = STAT.matchHblank();

        m_clock.wait(hblankCycles);
        // drawing scanline has finished,
        // now if not vertical blanking, scan other line

        if (STAT.matchFlag()) { // LY == LYC
          m_intr.LCDC_Status_IRQ = STAT.matchCoincidence();
        }

        if (ly() == screenHeight) {
          STAT.modeFlag(statMode::VERTICAL_BLANKING);
        } else {
          STAT.modeFlag(statMode::SEARCHING_OAM);
        }
        break;

      case statMode::VERTICAL_BLANKING: //////////////////////////////////// mode 1
        m_intr.VBlank_IRQ = STAT.matchVblank();

        for (uint8 i = 0; i < 10; ++i) { // 10 scanlines of vertical blank
          updateLY();
          m_clock.wait(scanlineCycles);
        }

        if (ly() == screenVBlankHeight) {
          resetLY();
        }

        STAT.modeFlag(statMode::SEARCHING_OAM);
        break;
    }
  } else { // lcdc off
    resetLY();
    STAT.modeFlag(statMode::VERTICAL_BLANKING);
  }
}

byte ppu::readVRAM(const std::size_t index) const noexcept {
  const std::size_t effective_index = 8_KB * vbk() + index; // 8KB is VRAM bank size
  return m_vram[effective_index];
}

void ppu::writeVRAM(const std::size_t index, const byte val) noexcept {
  const std::size_t effective_index = 8_KB * vbk() + index;
  m_vram[effective_index] = val;
}

byte ppu::readOAM(const std::size_t index) const noexcept { return m_oam[index]; }

void ppu::writeOAM(const std::size_t index, const byte val) noexcept { m_oam[index] = val; }

statMode ppu::status() const noexcept { return STAT.modeFlag(); }

void ppu::adjustVRAMSize() noexcept {
  if (color_gameboy_support) {
    m_vram.resize(16_KB);
  } else {
    m_vram.resize(8_KB);
  }
}

void ppu::fetchBackground() noexcept {
  const uint16 y = SCY + LY;

  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    if (y >= 0 && y < screenHeight && SCX + dx < screenWidth) {
      const uint16 x = SCX + dx;
      const uint16 tileIndex = (x / tileWidth) + ((y / tileHeight) * 32);

      const auto [tilemap, _] = LCDC.bgTilemapSelect();
      const byte attribute = readVRAM(tilemap + tileIndex);

      const auto [tileptrBase, isSigned] = LCDC.tilesetBasePtr();
      const uint16 tileptr = isSigned ? (tileptrBase + ((attribute - 128) * tileSize))
                                      : (tileptrBase + (attribute * tileSize));

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
        const uint8 cgbPalette = attribute & 0b0000'0111;
        m_frame[LY][dx] = cgb_bg_palette[cgbPalette][paletteIndex];
      } else {
        m_frame[LY][dx] = dmg_palette[BGP[paletteIndex]];
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
      const byte attribute = readVRAM(tilemap + tileIndex);

      // clang-format off
      const uint16 tileptr = isSigned 
	                     ? (tileptrBase + ((attribute - 128) * tileSize))
	                     : (tileptrBase + (attribute * tileSize));
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
        const uint8 cgbPalette = attribute & 0b0000'0111;
        m_frame[LY][dx] = cgb_bg_palette[cgbPalette][paletteIndex];
      } else {
        m_frame[LY][dx] = dmg_palette[BGP[paletteIndex]];
      }
    }
  }
}

void ppu::fetchSprite() noexcept {
  const uint8 maxPossibleSpritesPerScanline = 10;
  uint8 numberOfSpritesSoFar = 0;

  for (std::size_t oamIndex = 0; oamIndex < m_oam.size(); oamIndex += 4) {
    const uint8 spriteHeight = LCDC.spriteHeight();

    const byte yPos = readOAM(oamIndex) - 16; // sprite y position on screen
    const byte xPos = readOAM(oamIndex + 1) - 8;

    // clang-format off
    if ((yPos < LY) &&
       ((yPos + spriteHeight) >= LY) &&
       (xPos >= 0) &&
       (xPos < screenWidth)) {

      byte tileNumber =      readOAM(oamIndex + 2);
      const byte attribute = readOAM(oamIndex + 3);

      const flag bgHasPriority = attribute & 0b1000'0000;
      const flag yFlip =         attribute & 0b0100'0000;
      const flag xFlip =         attribute & 0b0010'0000;
      const uint8 dmgPalette =   attribute & 0b0001'0000;
      const uint8 vramBank =     attribute & 0b0000'1000;
      const uint8 cgbPalette =   attribute & 0b0000'0111;
      // clang-format on

      // if the background tile has priority, let the space used by it instead
      if (bgHasPriority) {
        continue;
      }

      uint8 tileRow = LY - yPos;
      if (yFlip) {
        tileRow = (spriteHeight - 1) - tileRow;
      }

      if (spriteHeight == 16) {
        if (tileRow >= 8) {
          tileRow -= 8;
          tileNumber |= 0b0000'0001;
        } else {
          tileNumber &= 0b1111'1110;
        }
      }

      if (++numberOfSpritesSoFar > maxPossibleSpritesPerScanline)
        break;

      const byte tilelineLowByte = readVRAM((tileNumber * tileSize) + (tileRow * 2));
      const byte tilelineHighByte = readVRAM((tileNumber * tileSize) + (tileRow * 2) + 1);

      // render a line of the sprite
      for (uint8 tileLineColumn = 0; tileLineColumn < tileWidth; ++tileLineColumn) {
        if (xPos + tileLineColumn >= screenWidth) {
          continue;
        }

        bool loBit, hiBit;
        if (xFlip) {
          loBit = (tilelineLowByte >> tileLineColumn) & 0b1;
          hiBit = (tilelineHighByte >> tileLineColumn) & 0b1;
        } else {
          loBit = (tilelineLowByte >> (7 - tileLineColumn)) & 0b1;
          hiBit = (tilelineHighByte >> (7 - tileLineColumn)) & 0b1;
        }

        const std::size_t paletteIndex = (hiBit << 1) | loBit;

        if (color_gameboy_support) {
          m_frame[LY][xPos + tileLineColumn] = cgb_sprite_palette[cgbPalette][paletteIndex];
        } else {
          // clang-format off
          if (paletteIndex == 0b00) continue; // transparent colors are not drawn on sprites

          if(dmgPalette == 1) m_frame[LY][xPos + tileLineColumn] = dmg_palette[OBP1[paletteIndex]];
          else                m_frame[LY][xPos + tileLineColumn] = dmg_palette[OBP0[paletteIndex]];
          // clang-format on
        }
      }
      /////
    }
  }
}

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

byte ppu::ly() const noexcept { return LY; }
void ppu::updateLY() noexcept { ++LY; }
void ppu::resetLY() noexcept { LY = 0; }

}
