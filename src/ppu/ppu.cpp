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

/*
Mode 2  2_____2_____2_____2_____2_____2___________________2____
Mode 3  _33____33____33____33____33____33__________________3___
Mode 0  ___000___000___000___000___000___000________________000
Mode 1  ____________________________________11111111111111_____
 */

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

      m_clock.cycle(vramCycles);
      STAT.mode_flag(stat::mode::HORIZONTAL_BLANKING);
    }

    if (STAT.mode_flag() == stat::mode::HORIZONTAL_BLANKING) { //// mode 0
      oam_accessible = true;
      vram_accessible = true;

      m_intr.LCDC_Status_IRQ = STAT.matchHblank();

      ++LY; // increase ly

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
  const auto [dataBlock, is_signed] = LCDC.bgChrBlockSelect();
  const auto [tile_map, _] = LCDC.chrMapAreaSelect();
  const uint8 y = LY - WY;
  const uint16 tile_row = y / tileHeight;

  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    const uint8 x = (dx >= WX) ? dx - WX : dx + WX;
    const uint16 tile_column = x / tileWidth;
    const uint16 address = tile_map + (tile_row * 32) + tile_column;
    const uint16 tile_number = is_signed ? int8(readVRAM(address)) : readVRAM(address);
    const uint16 tile_address = is_signed ? dataBlock + (tile_number * tileSize)
                                          : dataBlock + ((tile_number + 128) * tileSize);
    uint8 line_number = y % tileHeight;
    line_number *= 2;
    const byte lo = readVRAM(tile_address + line_number);
    const byte hi = readVRAM(tile_address + line_number + 1);
    const uint8 bit_check_mask = 7 - (x % 8); // start from left most bit and check
    const uint8 lo_bit = (lo >> bit_check_mask) & 0b1;
    const uint8 hi_bit = (hi >> bit_check_mask) & 0b1;
    const uint8 color_id = (lo_bit << 1) | hi_bit;
    const std::size_t palette_index = BGP.bgPalette(color_id);

    framebuffer[dx][LY] = default_palette[palette_index];

    m_clock.cycle(1);
  }
}

void ppu::fetchBG() {
  const auto [dataBlock, is_signed] = LCDC.bgChrBlockSelect();
  const auto [tile_map, _] = LCDC.bgMapAreaSelect();
  const uint8 y = SCY + LY;
  const uint16 tile_row = y / tileHeight;

  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    const uint8 x = SCX + dx;

    const uint16 tile_column = x / tileWidth;
    const uint16 address = tile_map + (tile_row * 32) + tile_column;
    const uint16 tile_number = is_signed ? int8(readVRAM(address)) : readVRAM(address);

    const uint16 tile_address = is_signed ? dataBlock + (tile_number * tileSize)
                                          : dataBlock + ((tile_number + 128) * tileSize);

    uint8 line_number = y % tileHeight;
    line_number *= 2;
    const byte lo = readVRAM(tile_address + line_number);
    const byte hi = readVRAM(tile_address + line_number + 1);

    // lo |7|6|5|4|3|2|1|0|
    // hi |7|6|5|4|3|2|1|0|
    const uint8 bit_check_mask = 7 - (x % 8); // start from left most bit and check
    const uint8 lo_bit = (lo >> bit_check_mask) & 0b1;
    const uint8 hi_bit = (hi >> bit_check_mask) & 0b1;
    const uint8 color_id = (lo_bit << 1) | hi_bit;
    const std::size_t palette_index = BGP.bgPalette(color_id);

    framebuffer[dx][LY] = default_palette[palette_index];
  }
}

void ppu::fetchOBJ() {
  // implement this
}

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
