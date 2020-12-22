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

void ppu::update(/*SDL_Renderer *renderer*/) {
  if (LCDC.lcdControllerStatus() == on) {
    if (STAT.mode_flag() == stat::mode::HORIZONTAL_BLANKING) {
      oam_accessable = true;
      vram_accessable = true;

      STAT.matchHblank(set);
      m_intr.stat_enabled = true;

      m_clock.cycle(22);
    }

    if (STAT.mode_flag() == stat::mode::VERTICAL_BLANKING) {
      oam_accessable = true;
      vram_accessable = true;

      STAT.matchVblank(set);
      m_intr.stat_enabled = true;

      m_clock.cycle(1140);
    }

    if (STAT.mode_flag() == stat::mode::SEARCHING_OAM) {
      oam_accessable = false;
      vram_accessable = true;

      STAT.matchSearchOAM(set);
      m_intr.stat_enabled = true;

      m_clock.cycle(20);
    }

    if (STAT.mode_flag() == stat::mode::TRANSFERING_DATA_TO_LCD) {
      oam_accessable = false;
      vram_accessable = false;

      m_clock.cycle(43);
    }

    if (STAT.match_flag()) { // LY == LYC
      STAT.matchCoincidence(set);
      m_intr.stat_enabled = true;
    }

    if (LY == 144) {
      m_intr.vblank_enabled = true;

      if (LY == 153) {
        LY = 0;
      }
    }
  } else {
    LY = 0;
    // draw idle color
  }
}

byte ppu::readVRAM(const std::size_t index) {
  if (vram_accessable) {
    return m_vram.at(index);
  }
}

void ppu::writeVRAM(const std::size_t index, const byte val) {
  if (vram_accessable) {
    m_vram.at(index) = val;
  }
}

byte ppu::readOAM(const std::size_t index) {
  if (oam_accessable) {
    return m_oam.at(index);
  }
}

void ppu::writeOAM(const std::size_t index, const byte val) {
  if (oam_accessable) {
    m_oam.at(index) = val;
  }
}

void ppu::scanline() {
  bool window_active = (LCDC.windowStatus() == on) && (LY >= WY);
  auto [dataBlock, is_signed] = LCDC.bgChrBlockSelect();
  auto [tile_map, _] = window_active ? LCDC.chrMapAreaSelect() : LCDC.bgMapAreaSelect();
  uint8 y = window_active ? LY - WY : LY + SCY;
  uint16 tile_row = y / tileHeight;

  for (uint8 dx = 0; dx < screenWidth; ++dx) {
    uint8 x = dx + SCX;
    if (window_active && (dx >= WX)) {
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

uint8 ppu::vbk() const noexcept { return cgb_support ? (VBK & 0b0000'0001) : 0; }
}
