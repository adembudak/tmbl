#include "tmbl/config.h"
#include "tmbl/clock/clock.h"

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

  for (int i = 0; i < n; ++i) {
    ++div_software_interface;
    DIV = div_software_interface >> 8;

    bool position = false;
    // clang-format off
      switch (TAC & 0b11) { // frequency
        case 0b00: position = div_software_interface & 0b0000'0010'0000'0000; break;
        case 0b01: position = div_software_interface & 0b0000'0000'0000'1000; break;
        case 0b10: position = div_software_interface & 0b0000'0000'0010'0000; break;
        case 0b11: position = div_software_interface & 0b0000'0000'1000'0000; break;
        // clang-format on
    }

    if (const auto edge = static_cast<edge_t>(position & isTimerEnabled());
        edge == edge_t::falling) {
      if (TIMA == std::numeric_limits<byte>::max()) {
        wait(1); // wait one cycle before assign TMA to TIMA
        TIMA = TMA;
        m_intr.Timer_Overflow_IRQ = set;
      } else {
        ++TIMA;
      }
    }
  }

  wait(n);
}

void clock::wait(const uint8 n) noexcept {
  using namespace std::this_thread;
  if (m_double_speed_mode) {
    sleep_for(double_speed_cycle_period{n});
  } else {
    sleep_for(normal_speed_cycle_period{n});
  }
}

void clock::resetDIV() noexcept { DIV = div_software_interface = 0; }

void clock::enableDoubleSpeedMode(const bool b) noexcept {
  m_double_speed_mode = b;
  resetDIV();
}

bool clock::isTimerEnabled() const noexcept { return TAC & 0b0000'0100; }

}
