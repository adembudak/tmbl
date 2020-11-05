#include "tmbl/ppu/internals/obp.h"
 // clang-format off

namespace tmbl {
class registers;

obp::obp(byte &val_) : m_value(val_) {}

int obp::objPalette(const uint8 val) {
  switch (val) {
    case 0: return m_value & 0b0000'0011;
    case 1: return (m_value & 0b0000'1100) >> 2;
    case 2: return (m_value & 0b0011'0000) >> 4;
    case 3: return (m_value & 0b1100'0000) >> 6;
    default: break;
  }
}

std::array<int, 4> obp::objPalette() {
    return { (m_value & 0b1100'0000) >> 6,
	     (m_value & 0b0011'0000) >> 4, 
	     (m_value & 0b0000'1100) >> 2, 
	     (m_value & 0b0000'0011) };
  }

}
