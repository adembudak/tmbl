#include "clock.h"
#include <thread>

namespace tmbl {

void clock::tick(const std::uint_fast8_t n) { std::this_thread::sleep_for(cycle_duration{n}); }

}

