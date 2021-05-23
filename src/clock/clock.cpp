#include "tmbl/config.h"
#include "tmbl/clock/clock.h"

#include <cstdio>
#include <thread>

/*
  Gameboy CPU runs at 4.194304 MHz in clock cycle
  and at 1.048576 MHz in machine cycle

  In double speed mode, it's 8.388608 MHz clock cycle
  and 2.097153 MHz in machine cycle

  This emu used machine cycles:
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

// clang-format off
clock::clock(registers &reg_, interrupts &intr):
        m_reg(reg_), 
        m_intr(intr), 
        DIV(m_reg.getAt(0xFF04)), TIMA(m_reg.getAt(0xFF05)), 
        TMA(m_reg.getAt(0xFF06)), TAC(m_reg.getAt(0xFF07)) {}
// clang-format on

void clock::cycle(const uint8 n) noexcept {
  using namespace std::this_thread;

  div_software_interface += n;
  general_counter_for_div += n;

  if (general_counter_for_div >= timerControllerAt(3)) {
    general_counter_for_div = 0;
    ++DIV;
  }

  uint16 position = 0;
  // clang-format off
  switch (const uint16 div_counter2 = ((div_software_interface & 0xff) | (DIV << 8)); TAC & 0b11) {
    case 0b00: position = div_counter2 & 0b0000'0001'0000'0000; break;
    case 0b01: position = div_counter2 & 0b0000'0000'0000'1000; break;
    case 0b10: position = div_counter2 & 0b0000'0000'0010'0000; break;
    case 0b11: position = div_counter2 & 0b0000'0000'1000'0000; break;
  }
  // clang-format on

  if (const bool isOnRisingEdge = position & isTimerEnabled(); !isOnRisingEdge) {
    if (TIMA == std::numeric_limits<byte>::max()) {
      sleep_for(normal_speed_cycle_period{1}); // wait one cycle before assign TMA to TIMA
      TIMA = TMA;
      m_intr.Timer_Overflow_IRQ = set;
      std::puts("m_intr.Timer_Overflow_IRQ = set;");
    } else {
      ++TIMA;
    }
  }

  if (m_double_speed_mode) {
    sleep_for(double_speed_cycle_period{n});
  } else {
    sleep_for(normal_speed_cycle_period{n});
  }
}

void clock::resetDIV() noexcept {
  DIV = 0;
  div_software_interface = 0;
}

void clock::enableDoubleSpeedMode(const bool b) noexcept {
  m_double_speed_mode = b;
  resetDIV();
}

bool clock::isTimerEnabled() const noexcept { return TAC & 0b0000'0100; }

uint16 clock::timerControllerAt(const uint8 val) const noexcept {
  // clang-format off
  switch (const uint16 current_freq = m_double_speed_mode ? (2 * m_base_freq) : m_base_freq; val) {
    case 0b00: return current_freq / 4'096;
    case 0b01: return current_freq / 263'144;
    case 0b10: return current_freq / 65'536;
    case 0b11: return current_freq / 16'384;
  }

}

uint16 clock::timerController() const noexcept {
  switch (const uint16 current_freq = m_double_speed_mode ? (2 * m_base_freq) : m_base_freq; TAC & 0b11 ) {
  // clang-format off
    case 0b00: return current_freq / 256;
    case 0b01: return current_freq / 4;
    case 0b10: return current_freq / 16;
    case 0b11: return current_freq / 64;
  }

}


}

