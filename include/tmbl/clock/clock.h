#ifndef CLOCK_H
#define CLOCK_H

#include "tmbl/config.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <ratio>
#include <chrono>

namespace tmbl {

class clock {
public:
  using normal_speed_frequency = std::ratio<1, 1048576>;
  using double_speed_frequency = std::ratio<1, 2097153>;

  using normal_speed_cycle_period = std::chrono::duration<double, normal_speed_frequency>;
  using double_speed_cycle_period = std::chrono::duration<double, double_speed_frequency>;

public:
  clock(registers &regs_, interrupts &intr_);

  void cycle(const uint8 n) noexcept;
  void wait(const uint8 n) const noexcept;
  void resetDIV() noexcept;
  void enableDoubleSpeedMode(const bool b) noexcept;

private:
  [[nodiscard]] bool isTimerEnabled() const noexcept;

private:
  registers &m_reg;
  interrupts &m_intr;

  enum class edge_t : uint8 { falling = 0, rising = 1 };

  uint16 div_software_interface = 0;
  bool m_double_speed_mode = false;
  int m_base_freq = 1'048'576; /* in Hertz */

  byte &DIV;
  byte &TIMA;
  byte &TMA;
  byte &TAC;
};

}

#endif
