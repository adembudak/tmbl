#ifndef STAT_H
#define STAT_H

#include "../../io/registers.h"
#include "../../config.h"
#include <memory>

namespace tmbl {

class stat {
public:
  stat(std::shared_ptr<registers> pRegs);

  // https://archive.org/details/GameBoyProgManVer1.1/page/n57/mode/1up
  enum class mode : uint8 {
    HORIZONTAL_BLANKING = 0b00,
    VERTICAL_BLANKING = 0b01,
    SEARCHING_OAM = 0b10,
    TRANSFERING_DATA_TO_LCD = 0b11,
  };

  mode mode_flag() const noexcept;
  void mode_flag(const mode m) noexcept;

  cflag match_flag() const noexcept;
  void match_flag(cflag val) noexcept;

  cflag matchHblank() { return STAT & 0b0000'1000; }
  cflag matchVblank() { return STAT & 0b0001'0000; }
  cflag matchSearchOAM() { return STAT & 0b0010'0000; }
  cflag matchCoincidence() { return STAT & 0b0100'0000; }

private:
  std::shared_ptr<registers> m_pRegs;
  byte &STAT = m_pRegs->getAt(0xFF41);
};

}

#endif
