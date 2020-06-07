#include "clock.h"

namespace tmbl::cpu {

void clock::tick(const std::uint_fast8_t n) { std::this_thread::sleep_for(cycle_duration{n}); }

}

