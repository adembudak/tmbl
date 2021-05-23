#ifndef CLOCK_H
#define CLOCK_H

#include "tmbl/config.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <ratio>
#include <chrono>

namespace tmbl {

class clock {
public:
  using normal_speed_frequency = std::ratio<1, 1048576>;
  using double_speed_frequency = std::ratio<1, 2097153>;

  using normal_speed_cycle_period = std::chrono::duration<double, normal_speed_frequency>;
  using double_speed_cycle_period = std::chrono::duration<double, double_speed_frequency>;

public:
  clock(const clock &other) noexcept = delete;
  clock &operator=(const clock &rhs) noexcept = delete;

  clock(clock &&other) noexcept = delete;
  clock &operator=(clock &&rhs) noexcept = delete;

  // singleton
  static clock &instance(registers &reg, interrupts &intr) {
    static clock c(reg, intr);
    return c;
  }

  void cycle(const uint8 n) noexcept;
  void resetDIV() noexcept;
  void enableDoubleSpeedMode(const bool b) noexcept;

private:
  explicit clock(registers &reg_, interrupts &intr);
  ~clock() = default;

  [[nodiscard]] bool isTimerEnabled() const noexcept;
  uint16 timerControllerAt(const uint8 val) const noexcept;
  uint16 timerController() const noexcept;

private:
  registers &m_reg;
  interrupts &m_intr;

  bool m_double_speed_mode = false;
  int m_base_freq = 1'048'576; /* in Hertz */

  uint16 div_counter = 0;
  bool is_freq_changed = false;

  byte &DIV;
  byte &TIMA;
  byte &TMA;
  byte &TAC;
};

}

#endif
