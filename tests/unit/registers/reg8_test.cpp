#include "tmbl/cpu/registers/reg8.h"
#include "tmbl/cpu/registers/reg16.h"
#include "tmbl/config.h"

#include <gtest/gtest.h>

using namespace tmbl;
using namespace tmbl::cpu;

TEST(EightBitRegisterType, Construction) {
  reg8 A;

  tmbl::u8 val = 0b0000'0000;
  ASSERT_TRUE(A.value() == val);

  reg8 B = A;
  ASSERT_EQ(B.value(), A.value());

  u8 b_data = B.value();
  u8 a_data = A.value();
  ASSERT_EQ(b_data, a_data);

  reg8 C;
  C = B;
  ASSERT_EQ(C.value(), B.value());
}

TEST(EightBitRegisterType, MemberOperatorEqualU8) {
  reg8 H;
  H = 224;
  ASSERT_EQ(H.value(), 224);
}

TEST(EightBitRegisterType, MemberOperatorEqualByte) {
  reg8 H;
  byte b = byte(128);

  H = b;
  ASSERT_EQ(H.value(), 128);
}

TEST(EightBitRegisterType, MemberOperatorPlusEqualU8) {
  reg8 H;
  H = 142;
  H += 11;
  ASSERT_EQ(H.value(), 153);
}

TEST(EightBitRegisterType, MemberOperatorMinusEqualU8) {
  reg8 H;
  H = 142;
  H -= 11;
  ASSERT_EQ(H.value(), 131);
}

TEST(EightBitRegisterType, MemberFuncTest) {
  reg8 A;

  A = 0b0100'0011U;

  ASSERT_EQ(A.test(0), true);
  ASSERT_EQ(A.test(1), true);
  ASSERT_EQ(A.test(2), false);
  ASSERT_EQ(A.test(3), false);

  ASSERT_EQ(A.test(4), false);
  ASSERT_EQ(A.test(5), false);
  ASSERT_EQ(A.test(6), true);
  ASSERT_EQ(A.test(7), false);
}

TEST(EightBitRegisterType, ZeroFlag) {
  reg8 F;

  F.Z(set);
  ASSERT_TRUE(F.Z() == set);
  ASSERT_EQ(F.value(), 0b1000'0000);

  F.Z(reset);
  ASSERT_EQ(F.Z(), reset);
  ASSERT_EQ(F.value(), 0b0000'0000);
}

TEST(EightBitRegisterType, NegationFlag) {
  reg8 F;

  F.N(set);
  ASSERT_EQ(F.N(), set);
  ASSERT_EQ(F.value(), 0b0100'0000);

  F.N(reset);
  ASSERT_EQ(F.N(), reset);
  ASSERT_EQ(F.value(), 0b0000'0000);
}

TEST(EightBitRegisterType, HalfCarryFlag) {
  reg8 F;

  F.H(set);
  ASSERT_EQ(F.H(), set);
  ASSERT_EQ(F.value(), 0b0010'0000);

  F.H(reset);
  ASSERT_EQ(F.H(), reset);
  ASSERT_EQ(F.value(), 0b0000'0000);
}

TEST(EightBitRegisterType, CarryFlag) {
  reg8 F;

  F.C(set);
  ASSERT_EQ(F.C(), set);
  ASSERT_EQ(F.value(), 0b0001'0000);

  F.C(reset);
  ASSERT_EQ(F.C(), reset);
  ASSERT_EQ(F.value(), 0b0000'0000);
}

TEST(EightBitRegisterType, Allofthem) {
  reg8 F;

  F.Z(set);
  F.N(set);
  F.H(set);
  F.C(set);

  ASSERT_EQ(F.Z(), set);
  ASSERT_EQ(F.N(), set);
  ASSERT_EQ(F.H(), set);
  ASSERT_EQ(F.C(), set);
  ASSERT_EQ(F.value(), 0b1111'0000);

  F.Z(reset);
  F.N(reset);
  F.H(reset);
  F.C(reset);
  ASSERT_EQ(F.value(), 0b0000'0000);
}

TEST(EightBitRegisterType, MemberLoHiNibble) {
  reg8 H;
  H = 0b1101'1001;

  ASSERT_EQ(H.loNibble(), 0b1001);
  ASSERT_EQ(H.hiNibble(), 0b1101);

  H = 0b1111'0000;

  ASSERT_EQ(H.loNibble(), 0b0000);
  ASSERT_EQ(H.hiNibble(), 0b1111);
}

TEST(EightBitRegisterType, MemberMSBandLSB) {
  reg8 H;
  H = 0b1001'0011;

  ASSERT_EQ(H.msb(), 1);
  ASSERT_EQ(H.lsb(), 1);

  H = 0b0001'0010;
  ASSERT_EQ(H.msb(), 0);
  ASSERT_EQ(H.lsb(), 0);
}

TEST(EightBitRegisterType, MemberMinandMax) {
  ASSERT_EQ(reg8::max(), 0b1111'1111);
  ASSERT_EQ(reg8::min(), 0b0000'0000);
}

TEST(EightBitRegisterType, NonmemberOperatorPlus) {
  reg8 B;
  reg8 C;

  B = 0b1111'0000;
  C = 0b0000'1111;
  reg8 result = B + C;
  ASSERT_EQ(result.value(), 0b1111'1111);
}

TEST(EightBitRegisterType, NonmemberOperatorMinus) {
  reg8 B;
  reg8 C;

  B = 0b1111'1111;
  C = 0b0000'0001;
  reg8 result = B - C;
  ASSERT_EQ(result.value(), 0b1111'1110);
}

TEST(EightBitRegisterType, NonmemberOperatorBinaryAnd) {
  reg8 B;
  reg8 C;

  B = 0b1111'1111;
  C = 0b0000'0001;
  reg8 result = B & C;
  ASSERT_EQ(result.value(), 0b0000'0001);
}

TEST(EightBitRegisterType, NonmemberOperatorBinaryOr) {
  reg8 B;
  reg8 C;

  B = 0b1111'0000;
  C = 0b0000'0001;
  reg8 result = B | C;
  ASSERT_EQ(result.value(), 0b1111'0001);
}

TEST(EightBitRegisterType, NonmemberOperatorBinaryXOR) {
  reg8 B;
  reg8 C;

  B = 0b1101'0001;
  C = 0b0000'0001;
  reg8 result = B ^ C;
  ASSERT_EQ(result.value(), 0b1101'0000);
}

