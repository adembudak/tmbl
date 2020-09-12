#ifndef CLOCK_H
#define CLOCK_H

#include "../config.h"

#include <chrono>
#include <ratio>

namespace tmbl {

class clock {

public:
  using normal_speed = std::ratio<954>;
  using double_speed = std::ratio<477>;

  using normal_speed_duration =
      std::chrono::duration<int, std::ratio_multiply<normal_speed, std::pico>>;

  using double_speed_duration =
      std::chrono::duration<int, std::ratio_multiply<double_speed, std::pico>>;

  void cycle(const uint8 n) noexcept;
  void enableDoubleSpeedMode(const bool val);

private:
  bool m_double_speed_mode = false;
};

}

#endif
