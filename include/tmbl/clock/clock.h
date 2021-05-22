#ifndef CLOCK_H
#define CLOCK_H

#include "tmbl/config.h"
#include "tmbl/io/registers.h"

#include <ratio>
#include <chrono>

/*
  Gameboy CPU runs at 4.194304 MHz in clock cycle
  and at 1.048576 MHz in machine cycle

  In double speed mode, it's 8.388608 MHz clock cycle
  and 2.097153 MHz in machine cycle

  This emulator used machine cycles:
  f = 1.048576 MHz = 1048576 Hz, T = 0.000953657 ms ~ 954 us
  f = 2.097153 MHz = 2097153 Hz, T = 0.000476837 ms ~ 477 us

Registers:

  - DIV (0xFF04): 8 bit counter. (read, on write resets to 0)

  - TIMA (0xFF05): 8 bit counter. Incremented at a frequency chosen by TAC. (read/write)

  - TMA (0xFF06): Assigns its value to TIMA when it's overflowed. (read/write)

  - TAC (0xFF07): Configures TIMA, decides whether the TIMA work or not, if
  it is, decides its frequency. (read/write)

*/

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
  static clock &instance(registers &r) {
    static clock c(r);
    return c;
  }

  void cycle(const uint8 n) noexcept;
  void enableDoubleSpeedMode(const bool b) noexcept;
  void resetDIV() noexcept;

private:
  explicit clock(registers &reg_);
  ~clock() = default;

private:
  [[nodiscard]] bool timerOn() const noexcept;
  uint16 timerController() const noexcept;

  registers &m_reg;
  bool m_double_speed_mode = false;
  int m_base_freq = 1'048'576; /* in Hertz */

  uint16 div_counter = 0;

  byte &DIV;
  byte &TIMA;
  byte &TMA;
  byte &TAC;
};

}

#endif
