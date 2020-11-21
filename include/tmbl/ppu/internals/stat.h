#ifndef STAT_H
#define STAT_H

#include "../../io/registers.h"
#include "../../config.h"

namespace tmbl {
// https://gbdev.io/pandocs/#ff41-stat-lcdc-status-r-w
// https://archive.org/details/GameBoyProgManVer1.1/page/n57/mode/1up

class stat {
public:
  explicit stat(byte &val_, byte &ly, byte &lyc);

  enum class mode : uint8 {
    HORIZONTAL_BLANKING = 0b00,
    VERTICAL_BLANKING = 0b01,
    SEARCHING_OAM = 0b10,
    TRANSFERING_DATA_TO_LCD = 0b11,
  };

  mode mode_flag() const noexcept;       // bit0 bit1
  void mode_flag(const mode m) noexcept; // bit0 bit1

  cflag match_flag() const noexcept;   // bit2
  void match_flag(cflag val) noexcept; // bit2

  cflag matchHblank() const noexcept;      // bit3
  cflag matchVblank() const noexcept;      // bit4
  cflag matchSearchOAM() const noexcept;   // bit5
  cflag matchCoincidence() const noexcept; // bit6

private:
  byte &m_value;

  byte &LY;
  byte &LYC;
};

}

#endif
