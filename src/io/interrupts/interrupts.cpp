#include "tmbl/config.h"
#include "tmbl/io/interrupts/interrupts.h"
// clang-format off

namespace tmbl {

byte interrupts::read(const std::size_t index) const noexcept {
  switch (index) {
    case 0xFF0F: // IF 
     return Button_Pressed_IRQ << 4 |
Serial_Transfer_Completion_IRQ << 3 |
            Timer_Overflow_IRQ << 2 |
               LCDC_Status_IRQ << 1 |
                    VBlank_IRQ;
    case 0xFFFF: // IE
     return Button_Pressed_Enabled << 4 | 
Serial_Transfer_Completion_Enabled << 3 | 
            Timer_Overflow_Enabled << 2 | 
               LCDC_Status_Enabled << 1 | 
                    VBlank_Enabled;
  }
}

void interrupts::write(const std::size_t index, const byte val) noexcept {
  switch (index) {
    case 0xFF0F: // IF
            Button_Pressed_IRQ = val & 0b001'0000;
Serial_Transfer_Completion_IRQ = val & 0b000'1000;
            Timer_Overflow_IRQ = val & 0b000'0100;
               LCDC_Status_IRQ = val & 0b000'0010;
                    VBlank_IRQ = val & 0b000'0001;
      break;

    case 0xFFFF: // IE
            Button_Pressed_Enabled = val & 0b0001'0000;
Serial_Transfer_Completion_Enabled = val & 0b0000'1000;
            Timer_Overflow_Enabled = val & 0b0000'0100;
               LCDC_Status_Enabled = val & 0b0000'0010;
                    VBlank_Enabled = val & 0b0000'0001;
      break;
  }
}

uint8 interrupts::IF() const noexcept { return read(0xFF0F); }
uint8 interrupts::IE() const noexcept { return read(0xFFFF); }

bool interrupts::isThereAnAwaitingInterrupt() const noexcept { return IE() & IF() & 0b0001'1111; }

}
