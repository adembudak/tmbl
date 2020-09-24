#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../../config.h"
#include <cstddef>

namespace tmbl {

class interrupts {
public:
  byte read(std::size_t index) noexcept;
  void write(const std::size_t index, const byte val) noexcept;

  byte IF{}; // interrupt request register
  byte IE{}; // interrupt enable register

  flag vertical_blanking_intr_requested;
  flag lcd_status_intr_requested;
  flag timer_overflowed_intr_requested;
  flag serial_completion_intr_requested;
  flag button_pressed_intr_requested;

  flag vertical_blanking_intr_enabled;
  flag lcd_status_intr_enabled;
  flag timer_overflowed_intr_enabled;
  flag serial_completion_intr_enabled;
  flag button_pressed_intr_enabled;
};

}

#endif

