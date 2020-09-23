#include "tmbl/config.h"
#include "tmbl/clock/clock.h"

#include <thread>

namespace tmbl {

void clock::cycle(const uint8 n) noexcept {
  if (m_double_speed_mode) {
    std::this_thread::sleep_for(double_speed_duration{n});
  } else {
    std::this_thread::sleep_for(normal_speed_duration{n});
  }
}

void clock::enableDoubleSpeedMode(const bool val) { m_double_speed_mode = val; }

}
