#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../../config.h"
#include <cstddef>

namespace tmbl {

// https://eldred.fr/gb-asm-tutorial/interrupts.html

class interrupts {

public:
  byte read(const std::size_t index) noexcept;
  void write(const std::size_t index, const byte val) noexcept;

  uint8 IF() const noexcept; // interrupt flag
  uint8 IE() const noexcept; // interrupt enabled

  bool vblank_pending = false; //  vertical blanking intr requested
  bool stat_pending = false;   //  lcd status intr requested
  bool timer_pending = false;  //  timer overflowed intr requested
  bool serial_pending = false; //  serial completion intr requested
  bool joypad_pending = false; //  button pressed intr requested

  bool vblank_enabled = false; //  vertical blanking intr enabled
  bool stat_enabled = false;   //  lcd status intr enabled
  bool timer_enabled = false;  //  timer overflowed intr enabled
  bool serial_enabled = false; //  serial completion intr enabled
  bool joypad_enabled = false; //  button pressed intr enabled
};

}

#endif

