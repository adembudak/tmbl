#include "tmbl/config.h"
#include "tmbl/clock/clock.h"

#include <thread>

namespace tmbl {

// clang-format off
clock::clock(registers &reg_):
        m_reg(reg_), 
        DIV(m_reg.getAt(0xFF04)), 
        TIMA(m_reg.getAt(0xFF05)), 
        TMA(m_reg.getAt(0xFF06)),
        TAC(m_reg.getAt(0xFF07)) {}
// clang-format on

void clock::cycle(const uint8 n) noexcept {
  using namespace std::this_thread;

  div_counter += n; // div counter incremented
  if (div_counter % 64 == 0) {
    ++DIV;
  }

  int position = 0;
  // clang-format off
  switch (TAC & 0b11) {
    case 0b00: position = div_counter & 0b0000'0001'0000'0000; break;
    case 0b01: position = div_counter & 0b0000'0000'0000'1000; break;
    case 0b10: position = div_counter & 0b0000'0000'0010'0000; break;
    case 0b11: position = div_counter & 0b0000'0000'1000'0000; break;
  }
  // clang-format on

  bool timerEnableBit = TAC & 0b0000'0100;
  bool rising_edge = position & timerEnableBit;

  if (!rising_edge) {
    ++TIMA;

    if (TIMA == std::numeric_limits<byte>::max()) {
      sleep_for(normal_speed_cycle_period{1});
      TIMA = TMA;
      // request request interrupt
      m_reg.getAt(0xFFFF) |= 0b0000'0100;
    }
  }

  if (m_double_speed_mode) {
    sleep_for(double_speed_cycle_period{n});
  } else {
    sleep_for(normal_speed_cycle_period{n});
  }
}

void clock::enableDoubleSpeedMode(const bool b) noexcept {
  m_double_speed_mode = b;
  resetDIV();
}

void clock::resetDIV() noexcept {
  DIV = 0; // ??
  div_counter = 0;
}

bool clock::timerOn() const noexcept { return TAC & 0b0000'0100; }

uint16 clock::timerController() const noexcept {
  // m_base_freq 1048576
  const uint16 current_freq = m_double_speed_mode ? (2 * m_base_freq) : m_base_freq;
  // clang-format off
  switch (TAC & 0b0000'0011) {
    case 0b00: return current_freq / 1024;
    case 0b01: return current_freq / 16 ;
    case 0b10: return current_freq / 64;
    case 0b11: return current_freq / 256;
  }
}
}

