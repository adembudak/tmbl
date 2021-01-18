#include "tmbl/config.h"
#include "tmbl/clock/clock.h"

#include <thread>

namespace tmbl {

// clang-format off
clock::clock(registers &reg_)
    : m_reg(reg_), 
    DIV(m_reg.getAt(0xFF04)), 
    TIMA(m_reg.getAt(0xFF05)), 
    TMA(m_reg.getAt(0xFF06)),
    TAC(m_reg.getAt(0xFF07)) {}
// clang-format on

void clock::cycle(const uint8 n) noexcept {
  using namespace std::this_thread;

  global_counter += n;
  div_counter += n;

  if (div_counter > 64) {
    div_counter = 0;
    DIV++;
  }

  if (m_double_speed_mode) {
    sleep_for(double_speed_cycle_period{n});
  } else {
    sleep_for(normal_speed_cycle_period{n});
  }
}

void clock::enableDoubleSpeedMode(const bool val) noexcept { m_double_speed_mode = val; }
bool clock::timerOn() const noexcept { return TAC & 0b0000'0100; }
void clock::resetDIV() noexcept { DIV = 0; }

uint16 clock::timerController() const noexcept {
  const uint16 freq = m_double_speed_mode ? 2 * m_base_freq : m_base_freq;

  // clang-format off
  switch (TAC & 0b0000'0011) {
    case 0b00: return freq / 1024; // 4096 Hz
    case 0b01: return freq / 16;   // 252144 Hz
    case 0b10: return freq / 64;   // 65536 Hz
    case 0b11: return freq / 256;  // 16384 Hz
  }
}
}

