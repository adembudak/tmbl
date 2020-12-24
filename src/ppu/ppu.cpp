#include "tmbl/config.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <array>
#include <cstddef>

namespace tmbl {

class registers;

// clang-format off
ppu::ppu(registers &regs_, cartridge &cart_, interrupts &intr_)
    : m_regs(regs_), m_cart(cart_), m_intr(intr_), cgb_support(m_cart.CGB()),
	STAT(m_regs.getAt(0xFF41), /*ly*/ m_regs.getAt(0xFF44), /*lyc*/m_regs.getAt(0xFF45)), 
        LCDC(m_regs.getAt(0xFF40), m_cart.CGB()),

        SCY(m_regs.getAt(0xFF42)), SCX(m_regs.getAt(0xFF43)), 

        LY(m_regs.getAt(0xFF44)), LYC(m_regs.getAt(0xFF45)), 

        DMA(m_regs.getAt(0xFF46)), 

        BGP(m_regs.getAt(0xFF47)), OBP0(m_regs.getAt(0xFF48)), OBP1(m_regs.getAt(0xFF49)), 

        WY(m_regs.getAt(0xFF4A)), WX(m_regs.getAt(0xFF4B)), 

        VBK(m_regs.getAt(0xFF4F)), 

        HDMA1(m_regs.getAt(0xFF51)), HDMA2(m_regs.getAt(0xFF52)), HDMA3(m_regs.getAt(0xFF53)), 
        HDMA4(m_regs.getAt(0xFF54)), HDMA5(m_regs.getAt(0xFF55)), 

        BCPS(m_regs.getAt(0xFF68)), BCPD(m_regs.getAt(0xFF69)), 
        OCPS(m_regs.getAt(0xFF6A)), OCPD(m_regs.getAt(0xFF6B))
// clang-format on
{}

void ppu::update() {
  if (LCDC.lcdControllerStatus() == on) {

    if (STAT.mode_flag() == stat::mode::SEARCHING_OAM) { //// mode 2
      oam_accessible = false;
      vram_accessible = true;

      m_intr.LCDC_Status_IRQ = STAT.matchSearchOAM();

      m_clock.cycle(oamCycles);
      STAT.mode_flag(stat::mode::TRANSFERING_DATA_TO_LCD);
    }

    if (STAT.mode_flag() == stat::mode::TRANSFERING_DATA_TO_LCD) { //// mode 3
      oam_accessible = false;
      vram_accessible = false;

      // draw: render framebuffer

      ++LY;

      m_clock.cycle(vramCycles);
      STAT.mode_flag(stat::mode::HORIZONTAL_BLANKING);
    }

    if (STAT.mode_flag() == stat::mode::HORIZONTAL_BLANKING) { //// mode 0
      oam_accessible = true;
      vram_accessible = true;

      m_intr.LCDC_Status_IRQ = STAT.matchHblank();

      m_clock.cycle(hblankCycles);
      STAT.mode_flag(stat::mode::SEARCHING_OAM);
    }

    if (LY == screenHeight) { //// mode 1
      oam_accessible = true;
      vram_accessible = true;

      m_intr.VBlank_IRQ = STAT.matchVblank();

      for (uint8 i = 0; i < 10; ++i) { // 10 scanlines of vertical blank
        ++LY;
        m_clock.cycle(scanlineCycles);
      }

      if (LY == screenVBlankHeight) {
        LY = 0;
      }

      STAT.mode_flag(stat::mode::SEARCHING_OAM);
    }

    if (STAT.match_flag()) { // LY == LYC
      m_intr.LCDC_Status_IRQ = STAT.matchCoincidence();
    }

  } else {
    LY = 0;
    STAT.mode_flag(stat::mode::VERTICAL_BLANKING);
  }
}

byte ppu::readVRAM(const std::size_t index) {
  if (vram_accessible) {
    return m_vram.at(index);
  }
}

void ppu::writeVRAM(const std::size_t index, const byte val) {
  if (vram_accessible) {
    m_vram.at(index) = val;
  }
}

byte ppu::readOAM(const std::size_t index) {
  if (oam_accessible) {
    return m_oam.at(index);
  }
}

void ppu::writeOAM(const std::size_t index, const byte val) {
  if (oam_accessible) {
    m_oam.at(index) = val;
  }
}

void ppu::fetchCHR() {
  auto [dataBlock, is_signed] = LCDC.bgChrBlockSelect();
  auto [tile_map, _] = LCDC.chrMapAreaSelect();
  uint8 y = LY - WY;
  uint16 tile_row = y / tileHeight;

  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    uint8 x = dx + SCX;
    if (dx >= WX) {
      x = dx - WX;
    }

    uint16 tile_column = x / tileWidth;
    uint16 address = tile_map + (tile_row * 32) + tile_column;
    uint16 tile_number = is_signed ? static_cast<int8>(readVRAM(address)) : readVRAM(address);

    uint16 tile_address = is_signed ? dataBlock + (tile_number * tileSize)
                                    : dataBlock + ((tile_number + 128) * tileSize);

    uint8 line_number = y % tileHeight;
    line_number *= 2;
    byte lo = readVRAM(tile_address + line_number);
    byte hi = readVRAM(tile_address + line_number + 1);

    // lo |7|6|5|4|3|2|1|0|
    // hi |7|6|5|4|3|2|1|0|
    uint8 bit_check_mask = 7 - (x % 8); // start from left most bit and check
    uint8 lo_bit = (lo >> bit_check_mask) & 0b1;
    uint8 hi_bit = (hi >> bit_check_mask) & 0b1;
    uint8 color_id = (lo_bit << 1) | hi_bit;
    std::size_t palette_index = BGP.bgPalette(color_id);

    framebuffer[dx][LY] = default_palette[palette_index];

    m_clock.cycle(1);
  }
}

void ppu::fetchBG() {

  auto [dataBlock, is_signed] = LCDC.bgChrBlockSelect();
  auto [tile_map, _] = LCDC.bgMapAreaSelect();
  uint8 y = LY + SCY;
  uint16 tile_row = y / tileHeight;

  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    uint8 x = dx + SCX;

    uint16 tile_column = x / tileWidth;
    uint16 address = tile_map + (tile_row * 32) + tile_column;
    uint16 tile_number = is_signed ? static_cast<int8>(readVRAM(address)) : readVRAM(address);

    uint16 tile_address = is_signed ? dataBlock + (tile_number * tileSize)
                                    : dataBlock + ((tile_number + 128) * tileSize);

    uint8 line_number = y % tileHeight;
    line_number *= 2;
    byte lo = readVRAM(tile_address + line_number);
    byte hi = readVRAM(tile_address + line_number + 1);

    // lo |7|6|5|4|3|2|1|0|
    // hi |7|6|5|4|3|2|1|0|
    uint8 bit_check_mask = 7 - (x % 8); // start from left most bit and check
    uint8 lo_bit = (lo >> bit_check_mask) & 0b1;
    uint8 hi_bit = (hi >> bit_check_mask) & 0b1;
    uint8 color_id = (lo_bit << 1) | hi_bit;
    std::size_t palette_index = BGP.bgPalette(color_id);

    framebuffer[dx][LY] = default_palette[palette_index];
  }
}

// are we drawing window or background or sprite.
// sprite on top of window, which on top of background
//
// first choose tile area
// second choose tile map
// third locate current scanline

// for i<-0 - 144
//  scanline(i);

// scanline(int i) {
// for k<-0 - 160
//   draw(framebuffer[i][k])
// }

// scanline called on data to lcd tranfer case on stat

// who calls the scaneline
// but what calls fetch function

// how to segreate gui from logic

void ppu::scanline() {

  if (LCDC.windowStatus() == on) {
    // (LY >= WY);
    fetchCHR();
  }

  if (LCDC.bgDisplayStatus() == on) {
    fetchBG();
  }

  if (LCDC.objDisplayStatus() == on) {
    fetchOBJ();
  }
}

uint8 ppu::vbk() const noexcept { return cgb_support ? (VBK & 0b0000'0001) : 0; }
}
