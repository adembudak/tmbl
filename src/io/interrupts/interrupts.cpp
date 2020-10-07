#include "tmbl/config.h"
#include "tmbl/io/interrupts/interrupts.h"

namespace tmbl {
    // clang-format off
byte interrupts::read(std::size_t index) noexcept {
  switch (index) {
    case 0xFF0F:
      IF = button_pressed_intr_requested << 4 | 
               lcd_status_intr_requested << 3 |
         timer_overflowed_intr_requested << 2 | 
        serial_completion_intr_requested << 1 |
                 button_pressed_intr_requested;
      return IF;

    case 0xFFFF:;
      IE = button_pressed_intr_requested << 4 | 
               lcd_status_intr_requested << 3 |
         timer_overflowed_intr_requested << 2 | 
        serial_completion_intr_requested << 1 |
                 button_pressed_intr_requested;
      return IE;
  }
}

void interrupts::write(const std::size_t index, const byte val) noexcept {
  switch (index) {
    case 0xFF0F:
      vertical_blanking_intr_requested = val & 0b000'0000;
      lcd_status_intr_requested =        val & 0b000'0001;
      timer_overflowed_intr_requested =  val & 0b000'0010;
      serial_completion_intr_requested = val & 0b000'0011;
      button_pressed_intr_requested =    val & 0b000'0100;

    case 0xFFFF:
      vertical_blanking_intr_enabled =  val & 0b000'0000;
      lcd_status_intr_enabled =         val & 0b000'0001;
      timer_overflowed_intr_enabled =   val & 0b000'0010;
      serial_completion_intr_enabled =  val & 0b000'0011;
      button_pressed_intr_enabled =     val & 0b000'0100;
  }
}

}
