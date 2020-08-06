#include "tmbl/ppu/ppu.h"

namespace tmbl {
ppu::ppu(std::shared_ptr<registers> pRegs, std::shared_ptr<cartridge> pCart)
    : m_pRegs(pRegs), m_pCart(pCart) {}

cflag ppu::bgDisplayStatus() const noexcept { return m_pCart->CGB() ? on : LCDC | 0b0000'0001; }

cflag ppu::objDisplayStatus() const noexcept { return LCDC & 0b0000'0010 ? on : off; }

std::pair<n8, n8> ppu::objSize() const noexcept {
  return LCDC & 0b0000'0100 ? std::make_pair(8, 16) : std::make_pair(8, 8);
}

std::pair<n16, n16> ppu::bgCodeArea() const noexcept {
  return LCDC & 0b0000'1000 ? std::make_pair(0x9C00, 0x9FFF) : std::make_pair(0x9800, 0x9BFF);
}

std::pair<n16, n16> ppu::bgChrArea() const noexcept {
  return LCDC & 0b0001'0000 ? std::make_pair(0x8000, 0x8FFF) : std::make_pair(0x8800, 0x97FF);
}

cflag ppu::windowStatus() const noexcept { return LCDC & 0b0010'0000 ? on : off; }

std::pair<n16, n16> ppu::windowCodeArea() const noexcept {
  return LCDC & 0b0100'0000 ? std::make_pair(0x9C00, 0x9FFF) : std::make_pair(0x9800, 0x9BFF);
}

cflag ppu::lcdControllerStatus() const noexcept { return LCDC & 0b1000'0000 ? on : off; }

ppu::mode ppu::mode_flag() const noexcept {
  switch (STAT | 0b0000'0011) {
    case 0:
      return mode::HORIZONTAL_BLANKING;
    case 1:
      return mode::VERTICAL_BLANKING;
    case 2:
      return mode::SEARCHING_OAM;
    case 3:
      return mode::TRANSFERING_DATA_TO_LCD;
  }
}

void ppu::mode_flag(const mode m) noexcept {
  switch (m) {
    case mode::HORIZONTAL_BLANKING:
      STAT |= 0b00;
    case mode::VERTICAL_BLANKING:
      STAT |= 0b01;
    case mode::SEARCHING_OAM:
      STAT |= 0b10;
    case mode::TRANSFERING_DATA_TO_LCD:
      STAT |= 0b11;
  }
}

cflag ppu::match_flag() const noexcept { return LY == LYC; }
void ppu::match_flag(cflag val) noexcept { val ? STAT |= 0b0000'0100 : STAT &= 0b1111'1011; }

}
