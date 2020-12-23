#include "tmbl/config.h"
#include "tmbl/io/interrupts/interrupts.h"
// clang-format off

namespace tmbl {

byte interrupts::read(const std::size_t index) noexcept {
  switch (index) {
    case 0xFF0F: // IF, interrupt request address
      return (vblank_pending << 4) |
               (stat_pending << 3) |
              (timer_pending << 2) |
             (serial_pending << 1) |
                  joypad_pending;

    case 0xFFFF: // IE, interrupt enable address
      return (vblank_enabled << 4) |
               (stat_enabled << 3) |
              (timer_enabled << 2) |
             (serial_enabled << 1) |
                  joypad_enabled;

     default: break;
  }
}

void interrupts::write(const std::size_t index, const byte val) noexcept {
  switch (index) {
    case 0xFF0F: // IF
      vblank_pending = val & 0b000'0000;
      stat_pending =   val & 0b000'0001;
      timer_pending =  val & 0b000'0010;
      serial_pending = val & 0b000'0011;
      joypad_pending = val & 0b000'0100;
      break;

    case 0xFFFF: // IE
      vblank_enabled = val & 0b000'0000;
      stat_enabled =   val & 0b000'0001;
      timer_enabled =  val & 0b000'0010;
      serial_enabled = val & 0b000'0011;
      joypad_enabled = val & 0b000'0100;
      break;

     default: break;
  }
}

uint8 interrupts::IF() const noexcept {
      return (vblank_pending << 4) |
               (stat_pending << 3) |
              (timer_pending << 2) |
             (serial_pending << 1) |
                  joypad_pending;
}

uint8 interrupts::IE() const noexcept {
      return (vblank_enabled << 4) |
               (stat_enabled << 3) |
              (timer_enabled << 2) |
             (serial_enabled << 1) |
                  joypad_enabled;
}

}

