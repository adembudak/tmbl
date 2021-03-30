#ifndef CLOCK_H
#define CLOCK_H

#include "../config.h"
#include "../io/registers.h"

#include <chrono>
#include <ratio>

/*
  Gameboy CPU runs at
  4.194304 MHz in terms of clock cycle and at 1.048576 MHz in machine cycle

  In double speed mode, it's
  8.388608 MHz clock cycle and 2.097153 MHz in machine cycle

  This emulator used machine cycles:
  f = 1.048576 MHz = 1048576 Hz, T = 0.000953657 ms ~ 954 us
  f = 2.097153 MHz = 2097153 Hz, T = 0.000476837 ms ~ 477 us
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
  void enableDoubleSpeedMode(const bool val) noexcept;
  void resetDIV() noexcept;

  /*
   // TODO: how to implement TIMA - TMA logic?
    if (TIMA > std::numeric_limits<byte>::max()) {
      TIMA = TMA;
      timer_interrupt_irc = set;
  */

private:
  explicit clock(registers &reg_);
  ~clock() = default;

private:
  [[nodiscard]] bool timerOn() const noexcept;
  uint16 timerController() const noexcept;

  registers &m_reg;
  bool m_double_speed_mode = false;
  uint16 m_base_freq = 4194304; /* in Hertz */

  uint64 global_counter = 0;
  uint8 div_counter = 0;

  byte &DIV;
  byte &TIMA;
  byte &TMA;
  byte &TAC;
};

}

#endif

