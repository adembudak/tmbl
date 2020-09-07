
#include "tmbl/ppu/registers/lcdc.h"

namespace tmbl {

lcdc::lcdc(std::shared_ptr<registers> pRegs) : m_pRegs(pRegs) {}

cflag lcdc::bgDisplayStatus() const noexcept {
  return /*m_pCart->CGB() ? on :*/ LCDC | 0b0000'0001;
}

cflag lcdc::objDisplayStatus() const noexcept { return LCDC & 0b0000'0010 ? on : off; }

std::pair<n8, n8> lcdc::objSize() const noexcept {
  return LCDC & 0b0000'0100 ? std::make_pair(8, 16) : std::make_pair(8, 8);
}

std::pair<n16, n16> lcdc::bgCodeArea() const noexcept {
  return LCDC & 0b0000'1000 ? std::make_pair(0x9C00, 0x9FFF) : std::make_pair(0x9800, 0x9BFF);
}

std::pair<n16, n16> lcdc::bgChrArea() const noexcept {
  return LCDC & 0b0001'0000 ? std::make_pair(0x8000, 0x8FFF) : std::make_pair(0x8800, 0x97FF);
}

cflag lcdc::windowStatus() const noexcept { return LCDC & 0b0010'0000 ? on : off; }

std::pair<n16, n16> lcdc::windowCodeArea() const noexcept {
  return LCDC & 0b0100'0000 ? std::make_pair(0x9C00, 0x9FFF) : std::make_pair(0x9800, 0x9BFF);
}

cflag lcdc::lcdControllerStatus() const noexcept { return LCDC & 0b1000'0000 ? on : off; }

}
