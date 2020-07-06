#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <ratio>
#include <cstdint>

namespace tmbl {

class clock final {
public:
  void tick(const std::uint_fast8_t n);

private:
  // Clock speed of GB processor is:
  // 1.05 MHz == 1.05*10^6 Hz == 1050000 Hz
  static constexpr intmax_t cpu_clock = 1050000;

  using cycle = std::ratio<1, cpu_clock>;
  using dcycle = std::ratio<1, cpu_clock * 2>;
  using cycle_duration = std::chrono::duration<std::uint_fast8_t, cycle>;
};
}

#endif

