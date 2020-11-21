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

void ppu::render(std::function<void(const uint8 x, const uint8 y, const color c)> draw, palette p) {

  if (LCDC.lcdControllerStatus() == on) { // lcdc.bit7

    for (uint8 dy = 0; dy < screenHeight; ++dy) {
      scanline(draw, p);

      STAT.mode_flag(stat::mode::HORIZONTAL_BLANKING);
    }
    STAT.mode_flag(stat::mode::VERTICAL_BLANKING);

  }

  else {
    // draw palette color 5
  }
}

void ppu::update() {
  if (LCDC.lcdControllerStatus() == on) {

    switch (STAT.mode_flag()) {
      case STAT.mode::SEARCHING_OAM: // mode 2
        // things
        STAT.mode_flag(STAT.mode::TRANSFERING_DATA_TO_LCD);
        break;
      case STAT.mode::TRANSFERING_DATA_TO_LCD: // mode 3
                                               // things
        STAT.mode_flag(STAT.mode::HORIZONTAL_BLANKING);
        break;
      case STAT.mode::HORIZONTAL_BLANKING: // mode 0
                                           // things
        STAT.mode_flag(STAT.mode::VERTICAL_BLANKING);
        break;
      case STAT.mode::VERTICAL_BLANKING: // mode 1
                                         // things
        STAT.mode_flag(STAT.mode::SEARCHING_OAM);
        break;
      default:
        break;
    }
  }
}

byte ppu::readVRAM(const std::size_t index) { return m_vram.at(index); }
void ppu::writeVRAM(const std::size_t index, const byte val) { m_vram.at(index) = val; }

byte ppu::readOAM(const std::size_t index) { return m_oam.at(index); }
void ppu::writeOAM(const std::size_t index, const byte val) { m_oam.at(index) = val; }

void ppu::scanline(std::function<void(const uint8 x, const uint8 y, const color c)> draw,
                   palette p) {

  for (uint8 dx = 0; dx < (screenWidth / tileWidth); ++dx) {

    // draw window (chr)
    if (LCDC.windowStatus() == on) {                     // lcdc.bit5
      auto [windowBeginAddress, _] = LCDC.chrCodeArea(); // lcdc.bit6

      // window is not scrollable, does not use scx, scy.
      auto block = LCDC.bgChrArea(); // lcdc.bit4

    }

    // draw background
    else if (LCDC.bgDisplayStatus() == on) { // lcdc.bit0

      uint8 scx = SCX / screenWidth; // wrap around
      uint8 scy = SCY / screenHeight;

      auto block = LCDC.bgChrArea();

      auto [bgBegin, _] = LCDC.bgCodeArea(); // lcdc.bit3

      // draw(scx, scy);
      // draw background
    }

    // draw obj (sprite)
    else if (LCDC.objDisplayStatus() == on) { // lcdc.bit 1
      auto [height, width] = LCDC.objSize();  // lcdc.bit2 used.
                                              // draw obj
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
