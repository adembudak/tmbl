#include "tmbl/ppu/internals/bgp.h"

namespace tmbl {

bgp::bgp(std::shared_ptr<registers> pRegs) : m_pRegs(pRegs){};

int bgp::bgPalette(const uint8 val) {
  switch (val) {
    case 0:
      return BGP & 0b0000'0011;
    case 1:
      return (BGP & 0b0000'1100) >> 2;
    case 2:
      return (BGP & 0b0011'0000) >> 4;
    case 3:
      return (BGP & 0b1100'0000) >> 6;
  }
}

std::array<int, 4> bgp::bgPalette() {
  // clang-format off
    return {(BGP & 0b1100'0000) >> 6,
	    (BGP & 0b0011'0000) >> 4, 
	    (BGP & 0b0000'1100) >> 2, 
	    (BGP & 0b0000'0011)};
  }

}
