#include "tmbl/config.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/cartridge/cartridge.h"

#include <array>
#include <cstddef>
#include <algorithm>

namespace tmbl {

class cartridge;
class interrupts;
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

    switch (STAT.mode_flag()) {
      case stat::mode::SEARCHING_OAM: // mode 2
                                      // things
        oam_accessable = false;
        vram_accessable = true;
        STAT.mode_flag(stat::mode::TRANSFERING_DATA_TO_LCD);
        break;

      case stat::mode::TRANSFERING_DATA_TO_LCD: // mode 3
                                                // things
        oam_accessable = false;
        vram_accessable = false;
        STAT.mode_flag(stat::mode::HORIZONTAL_BLANKING);
        break;

      case stat::mode::HORIZONTAL_BLANKING: // mode 0
                                            // things
        oam_accessable = true;
        vram_accessable = true;
        STAT.mode_flag(stat::mode::VERTICAL_BLANKING);
        break;

      case stat::mode::VERTICAL_BLANKING: // mode 1
                                          // things
        oam_accessable = true;
        vram_accessable = true;
        STAT.mode_flag(stat::mode::SEARCHING_OAM);
        break;

      default:
        break;
    }
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

void ppu::scanline(std::function<void(const uint8 x, const uint8 y, const color c)> draw) {

  for (uint8 dx = 0; dx < screenWidth; ++dx) {

    // draw window (chr)
    if (LCDC.windowStatus() == on) {                          // lcdc.bit5
      auto [windowBeginAddress, _] = LCDC.chrMapAreaSelect(); // lcdc.bit6

      // window is not scrollable, does not use scx, scy.
      auto block = LCDC.bgChrBlockSelect(); // lcdc.bit4

      if (cgb_support) {
        // use color gameboy palette
        //
      } else {
        // use bgp
        framebuffer[10][10] = default_palette[BGP.bgPalette(1)];
      }

    }

    // draw background
    else if (LCDC.bgDisplayStatus() == on) { // lcdc.bit0

      uint8 scy = SCY % screenHeight; // wrap around
      uint8 scx = SCX % screenWidth;

      auto [bgBegin, _] = LCDC.bgMapAreaSelect(); // lcdc.bit3
      auto [base_ptr, _t] = LCDC.bgChrBlockSelect();

      byte tile = 0x0;

      if (cgb_support) {
        auto [priority, yfliped, xfliped, chrbank, colorpalette] = [&tile]() {
          // https://archive.org/details/GameBoyProgManVer1.1/page/n53/mode/1up
          bool priority = tile & 0b1000'0000;
          bool yfliped = tile & 0b0100'0000;
          bool xfliped = tile & 0b0010'0000;
          bool chrbank = tile & 0b0000'1000;
          byte colorpalette = tile & 0b0000'0111;

          return std::make_tuple(priority, yfliped, xfliped, chrbank, colorpalette);
        }();
        // use bcps bcpd
      } else {
        //
        // use bgp
      }

      // draw(scx, scy);
    }

    // draw obj (sprite)
    else if (LCDC.objDisplayStatus() == on) {      // lcdc.bit 1
      auto [_ /*width*/, height] = LCDC.objSize(); // lcdc.bit2 used.
                                                   // draw obj
                                                   // sprites always uses 0x8000
      if (cgb_support) {
        // use ocps, ocpd
      } else {
        // oam palette selection flag 0->obp0, 1-> obp1
      }
    }
  }
}

std::array<uint8, 8> ppu::decode2BPP(const uint8 lo, const uint8 hi, const decodeMode mode) {
  std::array<uint8, 8> paletteIndexes{};
  uint8 mask = (mode == decodeMode::normal) ? 0b1000'0000 : 0b0000'0001;

  std::generate(paletteIndexes.begin(), paletteIndexes.end(), [&] {
    uint8 val = (hi & mask) | (lo & mask); // hi byte first, by 2bpp format
    if (mode == decodeMode::normal) {
      mask >>= 1;
    } else {
      mask <<= 1;
    }
    return val;
  });

  return paletteIndexes;
};

uint8 ppu::ly() const noexcept { return LY; }

void ppu::ly(const byte val) noexcept {
  LY = val;
  LY == LYC ? STAT.match_flag(set) : STAT.match_flag(reset);
}

uint8 ppu::vbk() const noexcept { return cgb_support ? (VBK & 0b0000'0001) : 0; }

}
