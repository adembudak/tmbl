#include "tmbl/config.h"
#include "tmbl/ppu/internals/stat.h"

#include <memory>

namespace tmbl {

class registers;

stat::stat(byte &val_) : m_value(val_) {}

stat::mode stat::mode_flag() const noexcept {
  // clang-format off
  switch (m_value & 0b0000'0011) {
    case 0: return mode::HORIZONTAL_BLANKING;
    case 1: return mode::VERTICAL_BLANKING;
    case 2: return mode::SEARCHING_OAM;
    case 3: return mode::TRANSFERING_DATA_TO_LCD;
    default: break;
  // clang-format on 
  }
}

void stat::mode_flag(const mode m) noexcept {
  switch (m) {
    case mode::HORIZONTAL_BLANKING:
      m_value |= 0b00;
      break;
    case mode::VERTICAL_BLANKING:
      m_value |= 0b01;
      break;
    case mode::SEARCHING_OAM:
      m_value |= 0b10;
      break;
    case mode::TRANSFERING_DATA_TO_LCD:
      m_value |= 0b11;
      break;
    default:
      break;
  }
}

cflag stat::match_flag() const noexcept { return false; /*return LY == LYC;*/ }

void stat::match_flag(cflag val) noexcept { val ? m_value |= 0b0000'0100 : m_value &= 0b1111'1011; }

};
