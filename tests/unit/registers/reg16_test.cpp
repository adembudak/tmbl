#include "tmbl/cpu/registers/reg8.h"
#include "tmbl/cpu/registers/reg16.h"
#include "tmbl/config.h"

#include <gtest/gtest.h>

using namespace tmbl;

TEST(SixteenBitRegisterType, memberOperatorEqualu16) {
  reg16 HL;
  HL = 0b1000'1000'1000'1000;
  ASSERT_EQ(HL.value(), 0b1000'1000'1000'1000);
}

TEST(SixteenBitRegisterType, memberOperatorPlusEqualu8) {
  reg16 HL;
  HL += 255;
  ASSERT_EQ(HL.value(), 0b0000'0000'1111'1111);
}

TEST(SixteenBitRegisterType, memberOperatorMinusEqualu8) {
  reg16 HL;
  HL += 255;
  HL -= 1;
  ASSERT_EQ(HL.value(), 0b0000'0000'1111'1110);
}

TEST(SixteenBitRegisterType, memberOperatorPostPlusPlus) {
  reg16 PC;

  PC++;
  ASSERT_EQ(PC.value(), 0b0000'0000'0000'0001);

  PC++;
  PC++;
  ASSERT_EQ(PC.value(), 0b0000'0000'0000'0011);
}

TEST(SixteenBitRegisterType, memberLoHiAccessor) {
  reg16 HL;
  HL = 0b1001'1101'0011'1011;

  byte lo = HL.lo();
  ASSERT_EQ(lo, 0b0011'1011);

  byte hi = HL.hi();
  ASSERT_EQ(hi, 0b1001'1101);
}

TEST(SixteenBitRegisterType, memberLoHiSetter) {
  reg16 HL;
  HL = 0b1111'1111'0000'0000;

  byte b{0b1100'0011};
  HL.lo(b);
  ASSERT_EQ(HL.value(), 0b1111'1111'1100'0011);

  b = 0b1001'1010;
  HL.hi(b);
  ASSERT_EQ(HL.value(), 0b1001'1010'1100'0011);

  reg16 BC;
  BC.hi(0b1010'1010);
  BC.lo(0b0110'0110);
  ASSERT_EQ(BC.value(), 0b1010'1010'0110'0110);
}

TEST(SixteenBitRegisterType, NonMemberOperatorPlusMinusReg16andInt) {
  reg16 PC;

  u16 nn = PC + 1;
  ASSERT_EQ(nn, 1);

  nn = PC + 255;
  ASSERT_EQ(nn, 255);

  PC = 256;

  nn = PC - 1;
  ASSERT_EQ(nn, 255);

  nn = PC - 256;
  ASSERT_EQ(nn, 0);
}
