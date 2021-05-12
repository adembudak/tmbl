#include "tmbl/config.h"
#include "tmbl/io/interrupts/interrupts.h"
// clang-format off

namespace tmbl {

byte interrupts::read(const std::size_t index) const noexcept {
  switch (index) {
    case 0xFF0F: // IF, interrupt request address
             return (VBlank_IRQ << 4) | 
               (LCDC_Status_IRQ << 3) | 
            (Timer_Overflow_IRQ << 2) |
(Serial_Transfer_Completion_IRQ << 1) | 
                  Button_Pressed_IRQ;

    case 0xFFFF: // IE, interrupt enable address
             return (VBlank_Enabled << 4) | 
	       (LCDC_Status_Enabled << 3) | 
            (Timer_Overflow_Enabled << 2) |
(Serial_Transfer_Completion_Enabled << 1) | 
                  Button_Pressed_Enabled;

    default: break;
  }
}

void interrupts::write(const std::size_t index, const byte val) noexcept {
  switch (index) {
    case 0xFF0F: // IF
                    VBlank_IRQ = val & 0b000'0000;
               LCDC_Status_IRQ = val & 0b000'0001;
            Timer_Overflow_IRQ = val & 0b000'0010;
Serial_Transfer_Completion_IRQ = val & 0b000'0011;
            Button_Pressed_IRQ = val & 0b000'0100;
      break;

    case 0xFFFF: // IE
                    VBlank_Enabled = val & 0b000'0000;
               LCDC_Status_Enabled = val & 0b000'0001;
            Timer_Overflow_Enabled = val & 0b000'0010;
Serial_Transfer_Completion_Enabled = val & 0b000'0011;
            Button_Pressed_Enabled = val & 0b000'0100;
      break;

    default: break;
  }
}

uint8 interrupts::IF() const noexcept {
                 return (VBlank_IRQ << 4) | 
                   (LCDC_Status_IRQ << 3) | 
                (Timer_Overflow_IRQ << 2) |
    (Serial_Transfer_Completion_IRQ << 1) | 
                      Button_Pressed_IRQ;
}

uint8 interrupts::IE() const noexcept {
             return (VBlank_Enabled << 4) | 
               (LCDC_Status_Enabled << 3) | 
            (Timer_Overflow_Enabled << 2) |
 (Serial_Transfer_Completion_Enabled<< 1) | 
                 Button_Pressed_Enabled;
}

}

