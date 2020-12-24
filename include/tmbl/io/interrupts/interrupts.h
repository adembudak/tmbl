#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../../config.h"
#include <cstddef>

namespace tmbl {

// https://eldred.fr/gb-asm-tutorial/interrupts.html
// https://archive.org/details/GameBoyProgManVer1.1/page/n25/mode/1up;w

class interrupts {

public:
  byte read(const std::size_t index) noexcept;
  void write(const std::size_t index, const byte val) noexcept;

  uint8 IF() const noexcept; // 0xFF0F, interrupt flag
  uint8 IE() const noexcept; // 0xFFFF, interrupt enabled

  bool VBlank_IRQ = false;                     //  vertical blanking intr requested
  bool LCDC_Status_IRQ = false;                //  lcd status intr requested
  bool Timer_Overflow_IRQ = false;             //  timer overflowed intr requested
  bool Serial_Transfer_Completion_IRQ = false; //  serial completion intr requested
  bool Button_Pressed_IRQ = false;             //  button pressed intr requested

  bool VBlank_Enabled = false;                     //  vertical blanking intr enabled
  bool LCDC_Status_Enabled = false;                //  lcd status intr enabled
  bool Timer_Overflow_Enabled = false;             //  timer overflowed intr enabled
  bool Serial_Transfer_Completion_Enabled = false; //  serial completion intr enabled
  bool Button_Pressed_Enabled = false;             //  button pressed intr enabled
};

}

#endif

