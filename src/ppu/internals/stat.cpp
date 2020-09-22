#include "tmbl/ppu/internals/stat.h"

namespace tmbl {

stat::stat(std::shared_ptr<registers> pRegs) : m_pRegs(pRegs) {}

stat::mode stat::mode_flag() const noexcept {
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

void stat::mode_flag(const mode m) noexcept {
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

cflag stat::match_flag() const noexcept { return false; /*return LY == LYC;*/ }

void stat::match_flag(cflag val) noexcept { val ? STAT |= 0b0000'0100 : STAT &= 0b1111'1011; }

};
