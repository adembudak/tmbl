#include "tmbl/ppu/internals/bgp.h"

namespace tmbl {
class registers;

bgp::bgp(byte &val_) : m_value(val_) {}

std::size_t bgp::operator[](const std::size_t val) const noexcept {
  // clang-format off
  switch (val) {
    case 0: return m_value & 0b0000'0011;
    case 1: return (m_value & 0b0000'1100) >> 2;
    case 2: return (m_value & 0b0011'0000) >> 4;
    case 3: return (m_value & 0b1100'0000) >> 6;
    default: break;
  }
}
}
