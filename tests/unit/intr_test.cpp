#include "tmbl/config.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <gtest/gtest.h>

namespace {
using namespace tmbl;
interrupts intr;

TEST(intr, DefaultInterruptRequestState) {
  EXPECT_FALSE(intr.Button_Pressed_IRQ);
  EXPECT_FALSE(intr.Serial_Transfer_Completion_IRQ);
  EXPECT_FALSE(intr.Timer_Overflow_IRQ);
  EXPECT_FALSE(intr.LCDC_Status_IRQ);
  EXPECT_FALSE(intr.VBlank_IRQ);
}

TEST(intr, DefaultInterruptEnableState) {
  EXPECT_FALSE(intr.Button_Pressed_Enabled);
  EXPECT_FALSE(intr.Serial_Transfer_Completion_Enabled);
  EXPECT_FALSE(intr.Timer_Overflow_Enabled);
  EXPECT_FALSE(intr.LCDC_Status_Enabled);
  EXPECT_FALSE(intr.VBlank_Enabled);
}

TEST(intr, EnableAndRequestInterrupt) {
  bool interrupt_exist = intr.IE() && intr.IF() && 0b0001'1111;
  EXPECT_FALSE(interrupt_exist);

  intr.Button_Pressed_IRQ = true;
  EXPECT_TRUE(intr.Button_Pressed_IRQ);
  EXPECT_EQ(intr.read(0xFF0F), 0b0001'0000);
  EXPECT_EQ(intr.IF(), 0b0001'0000);

  intr.Button_Pressed_Enabled = true;
  EXPECT_TRUE(intr.Button_Pressed_Enabled);
  EXPECT_EQ(intr.read(0xFFFF), 0b0001'0000);
  EXPECT_EQ(intr.IE(), 0b0001'0000);

  interrupt_exist = intr.IE() && intr.IF() && 0b0001'1111;
  EXPECT_TRUE(interrupt_exist);
}

TEST(intr, interruptWriteIF) {
  intr.write(0xFF0F /*IF*/, 0b0'0000);
  EXPECT_EQ(intr.read(0xFF0F), 0b0'0000);

  EXPECT_EQ(intr.Button_Pressed_IRQ, false);
  EXPECT_EQ(intr.Serial_Transfer_Completion_IRQ, false);
  EXPECT_EQ(intr.Timer_Overflow_IRQ, false);
  EXPECT_EQ(intr.LCDC_Status_IRQ, false);
  EXPECT_EQ(intr.VBlank_IRQ, false);

  intr.write(0xFF0F, 0b1'0001);
  EXPECT_EQ(intr.read(0xFF0F), 0b1'0001);

  EXPECT_EQ(intr.Button_Pressed_IRQ, true);
  EXPECT_EQ(intr.Serial_Transfer_Completion_IRQ, false);
  EXPECT_EQ(intr.Timer_Overflow_IRQ, false);
  EXPECT_EQ(intr.LCDC_Status_IRQ, false);
  EXPECT_EQ(intr.VBlank_IRQ, true);

  intr.write(0xFF0F, 0b0'1110);
  EXPECT_EQ(intr.read(0xFF0F), 0b0'1110);

  EXPECT_EQ(intr.Button_Pressed_IRQ, false);
  EXPECT_EQ(intr.Serial_Transfer_Completion_IRQ, true);
  EXPECT_EQ(intr.Timer_Overflow_IRQ, true);
  EXPECT_EQ(intr.LCDC_Status_IRQ, true);
  EXPECT_EQ(intr.VBlank_IRQ, false);
}

TEST(intr, interruptWriteIE) {
  intr.write(0xFFFF /*IE*/, 0b0'0000);
  EXPECT_EQ(intr.read(0xFFFF), 0b0'0000);

  EXPECT_EQ(intr.Button_Pressed_Enabled, false);
  EXPECT_EQ(intr.Serial_Transfer_Completion_Enabled, false);
  EXPECT_EQ(intr.Timer_Overflow_Enabled, false);
  EXPECT_EQ(intr.LCDC_Status_Enabled, false);
  EXPECT_EQ(intr.VBlank_Enabled, false);

  intr.write(0xFFFF, 0b1'0001);
  EXPECT_EQ(intr.read(0xFFFF), 0b1'0001);

  EXPECT_EQ(intr.Button_Pressed_Enabled, true);
  EXPECT_EQ(intr.Serial_Transfer_Completion_Enabled, false);
  EXPECT_EQ(intr.Timer_Overflow_Enabled, false);
  EXPECT_EQ(intr.LCDC_Status_Enabled, false);
  EXPECT_EQ(intr.VBlank_Enabled, true);

  intr.write(0xFFFF, 0b0'1110);
  EXPECT_EQ(intr.read(0xFFFF), 0b0'1110);

  EXPECT_EQ(intr.Button_Pressed_Enabled, false);
  EXPECT_EQ(intr.Serial_Transfer_Completion_Enabled, true);
  EXPECT_EQ(intr.Timer_Overflow_Enabled, true);
  EXPECT_EQ(intr.LCDC_Status_Enabled, true);
  EXPECT_EQ(intr.VBlank_Enabled, false);
}
}
