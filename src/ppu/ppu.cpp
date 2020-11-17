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
	STAT(m_regs.getAt(0xFF41)), LCDC(m_regs.getAt(0xFF40), m_cart.CGB()),

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

// 7 6 5 3 2 1 0
void ppu::render(std::function<void(const uint8 x, const uint8 y, const color c)> draw, palette p) {

  if (LCDC.lcdControllerStatus() == on) { // lcdc.bit7

    for (uint8 dy = WY; dy < screenHeight; ++dy) {
      for (uint8 dx = WX + 7; dx < screenWidth + 7; ++dx) {

        uint8 scx = SCX / screenWidth;
        uint8 scy = SCY / screenHeight;

        if (LCDC.windowStatus() == on) {              // lcdc.bit5
          auto [windowBegin, _] = LCDC.chrCodeArea(); // lcdc.bit6

          auto block = LCDC.bgChrArea();

          byte tile_lo = m_vram.at(windowBegin + scx);
          byte tile_hi = m_vram.at(windowBegin + scx + 1);

          auto tile_line_colors = decode2BPP(tile_lo, tile_hi);

          draw(scx, scy, p[tile_line_colors[0]]);
        }

        else if (LCDC.bgDisplayStatus() == on) { // lcdc.bit0
          auto [bgBegin, _] = LCDC.bgCodeArea(); // lcdc.bit3
          auto block = LCDC.bgChrArea();

          // draw background
        }

        else if (LCDC.objDisplayStatus() == on) { // lcdc.bit 1
          auto [height, width] = LCDC.objSize();  // lcdc.bit2 used.
          // draw obj
        }
      }

      STAT.mode_flag(stat::mode::HORIZONTAL_BLANKING);
    }
    STAT.mode_flag(stat::mode::VERTICAL_BLANKING);

  }

  else {
    // draw palette color 5
  }
}

byte ppu::readVRAM(const std::size_t index) { return m_vram.at(index); }
void ppu::writeVRAM(const std::size_t index, const byte val) { m_vram.at(index) = val; }

byte ppu::readOAM(const std::size_t index) { return m_oam.at(index); }
void ppu::writeOAM(const std::size_t index, const byte val) { m_oam.at(index) = val; }

std::array<uint8, 8> ppu::decode2BPP(const uint8 lo, const uint8 hi, const decodeMode mode) {

  uint8 mask = (mode == decodeMode::normal) ? 0b1000'0000 : 0b0000'0001;

  std::array<uint8, 8> decodedTileLine{};

  std::transform(decodedTileLine.begin(), decodedTileLine.end(), decodedTileLine.begin(),
                 [&](const uint8 /*unused*/) {
                   uint8 val = 0;
                   if (mode == decodeMode::normal) {
                     val = (hi & mask) | (lo & mask); // hi byte first, by 2bpp format
                     mask >>= 1;
                   } else {
                     val = (hi & mask) | (lo & mask);
                     mask <<= 1;
                   }
                   return val;
                 });

  return decodedTileLine;
};

uint8 ppu::ly() const noexcept { return LY; }

void ppu::ly(const byte val) noexcept {
  LY = val;
  LY == LYC ? STAT.match_flag(set) : STAT.match_flag(reset);
}

uint8 ppu::vbk() const noexcept { return VBK & 0b0000'0001 ? 1 : 0; }

}

