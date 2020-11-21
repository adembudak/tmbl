#include "tmbl/config.h"
#include "tmbl/ppu/internals/stat.h"

namespace tmbl {

class registers;

stat::stat(byte &val_, byte &ly, byte &lyc) : m_value(val_), LY(ly), LYC(lyc) {}

stat::mode stat::mode_flag() const noexcept {

  switch (m_value & 0b0000'0011) {
      // clang-format off
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
      m_value = (m_value & 0b1111'1100); // 0b00
      break;

    case mode::VERTICAL_BLANKING:
      m_value = (m_value & 0b1111'1100) | 1; // 0b01
      break;

    case mode::SEARCHING_OAM:
      m_value = (m_value & 0b1111'1100) | 2; // 0b10
      break;

    case mode::TRANSFERING_DATA_TO_LCD:
      m_value = (m_value & 0b1111'1100) | 3; // 0b11
      break;

    default:
      break;
  }
}

cflag stat::match_flag() const noexcept { return LY == LYC; }
void stat::match_flag(cflag val) noexcept { val ? m_value |= 0b0000'0100 : m_value &= 0b1111'1011; }

cflag stat::matchHblank() const noexcept { return m_value & 0b0000'1000; }
cflag stat::matchVblank() const noexcept { return m_value & 0b0001'0000; }
cflag stat::matchSearchOAM() const noexcept { return m_value & 0b0010'0000; }
cflag stat::matchCoincidence() const noexcept { return m_value & 0b0100'0000; }

}
