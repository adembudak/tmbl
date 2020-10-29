#ifndef STAT_H
#define STAT_H

#include "../../io/registers.h"
#include "../../config.h"
#include <memory>

namespace tmbl {

class stat {
public:
  explicit stat(byte &val_);

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

  cflag matchHblank() { return m_value & 0b0000'1000; }
  cflag matchVblank() { return m_value & 0b0001'0000; }
  cflag matchSearchOAM() { return m_value & 0b0010'0000; }
  cflag matchCoincidence() { return m_value & 0b0100'0000; }

private:
  std::shared_ptr<registers> m_pRegs;
  byte &m_value;
};

}

#endif

