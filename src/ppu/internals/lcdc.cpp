#include "tmbl/ppu/internals/lcdc.h"

namespace tmbl {

class registers;

lcdc::lcdc(byte &val_, bool cgb_support) : m_value(val_), cgb_support(cgb_support) {}

cflag lcdc::lcdControllerStatus() const noexcept { return m_value & 0b1000'0000 ? on : off; }

std::pair<uint16, uint16> lcdc::chrMapAreaSelect() const noexcept {
  return m_value & 0b0100'0000 ? std::make_pair(0x1C00, 0x1FFF) : std::make_pair(0x1800, 0x1BFF);
}

cflag lcdc::windowStatus() const noexcept { return m_value & 0b0010'0000 ? on : off; }

std::pair<uint16, uint16> lcdc::bgChrBlockSelect() const noexcept {
  return m_value & 0b0001'0000 ? std::make_pair(0x0000, 0x0FFF) : std::make_pair(0x0800, 0x17FF);
}

std::pair<uint16, uint16> lcdc::bgMapAreaSelect() const noexcept {
  return m_value & 0b0000'1000 ? std::make_pair(0x1C00, 0x1FFF) : std::make_pair(0x1800, 0x1BFF);
}

std::pair<uint8, uint8> lcdc::objSize() const noexcept {
  return m_value & 0b0000'0100 ? std::make_pair(8, 16) : std::make_pair(8, 8);
}

cflag lcdc::objDisplayStatus() const noexcept { return m_value & 0b0000'0010 ? on : off; }

cflag lcdc::bgDisplayStatus() const noexcept { return cgb_support ? on : m_value & 0b0000'0001; }

}

