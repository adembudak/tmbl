#ifndef STAT_H
#define STAT_H

#include "tmbl/io/registers.h"
#include "tmbl/config.h"

namespace tmbl {
// https://gbdev.io/pandocs/#ff41-stat-lcdc-status-r-w
// https://archive.org/details/GameBoyProgManVer1.1/page/n57/mode/1up

enum class statMode : uint8 {
  HORIZONTAL_BLANKING = 0b00,
  VERTICAL_BLANKING = 0b01,
  SEARCHING_OAM = 0b10,
  TRANSFERING_DATA_TO_LCD = 0b11,
};

class stat {
public:
  explicit stat(byte &val_, byte &ly, byte &lyc);

  statMode modeFlag() const noexcept;       // bit0 bit1
  void modeFlag(const statMode m) noexcept; // bit0 bit1

  flag matchFlag() const noexcept;    // bit2
  void matchFlag(cflag val) noexcept; // bit2

  bool matchHblank() const noexcept;      // bit3
  bool matchVblank() const noexcept;      // bit4
  bool matchSearchOAM() const noexcept;   // bit5
  bool matchCoincidence() const noexcept; // bit6

private:
  byte &m_value;

  byte &LY;
  byte &LYC;
};

}

#endif
