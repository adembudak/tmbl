#ifndef CLOCK_H
#define CLOCK_H

#include "../config.h"

#include <chrono>
#include <ratio>

/*
  Gameboy CPU runs at
  4.194304 MHz in terms of clock cycle and at 1.048576 MHz in machine cycle

  In double speed mode, it's
  8.388608 MHz in terms of clock cycle and at 2.097153 MHz in machine cycle

  This emulator used machine cycle:
  f = 1.048576 MHz = 1048576 Hz, T = 0.000953657 ms ~ 954 us
  f = 2.097153 MHz = 2097153 Hz, T = 0.000476837 ms - 477 us
 */
namespace tmbl {

class clock {

public:
  using normal_speed = std::ratio<954>;
  using double_speed = std::ratio<477>;

  using normal_speed_duration =
      std::chrono::duration<int, std::ratio_multiply<normal_speed, std::micro>>;

  using double_speed_duration =
      std::chrono::duration<int, std::ratio_multiply<double_speed, std::micro>>;

  void cycle(const uint8 n) noexcept;
  void enableDoubleSpeedMode(const bool val);

private:
  bool m_double_speed_mode = false;
};

}

#endif

