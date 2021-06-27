#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "tmbl/config.h"
#include <cstddef>

namespace tmbl {

// https://eldred.fr/gb-asm-tutorial/interrupts.html
// https://archive.org/details/GameBoyProgManVer1.1/page/n25/mode/1up

class interrupts {

public:
  byte read(const std::size_t index) const noexcept;
  void write(const std::size_t index, const byte val) noexcept;

  uint8 IF() const noexcept; // 0xFF0F, interrupt request flag
  uint8 IE() const noexcept; // 0xFFFF, interrupt enabled flag

  bool isThereAnAwaitingInterrupt() const noexcept;

  bool Button_Pressed_IRQ = false;             //  button pressed intr requested, bit4
  bool Serial_Transfer_Completion_IRQ = false; //  serial completion intr requested, bit3
  bool Timer_Overflow_IRQ = false;             //  timer overflowed intr requested, bit2
  bool LCDC_Status_IRQ = false;                //  lcd status intr requested, bit1
  bool VBlank_IRQ = false;                     //  vertical blanking intr requested, bit0

  bool Button_Pressed_Enabled = false;             //  button pressed intr enabled, bit4
  bool Serial_Transfer_Completion_Enabled = false; //  serial completion intr enabled, bit3
  bool Timer_Overflow_Enabled = false;             //  timer overflowed intr enabled, bit2
  bool LCDC_Status_Enabled = false;                //  lcd status intr enabled, bit1
  bool VBlank_Enabled = false;                     //  vertical blanking intr enabled, bit0
};

}

#endif
