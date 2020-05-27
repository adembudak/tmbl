#include "tmbl/cpu/registers/reg8.h"
#include "tmbl/cpu/registers/reg16.h"
#include <tmbl/config.h>

#include <gtest/gtest.h>

using namespace tmbl;
using namespace tmbl::cpu;

TEST(EightBitRegisterType, Construction) {
  reg8 A;

  tmbl::uint8 val = 0b0000'0000;
  ASSERT_TRUE(A == val);

  reg8 B = A;
  ASSERT_EQ(B, A);

  auto b_data = B.data();
  auto a_data = A.data();
  ASSERT_EQ(b_data, a_data);

  reg8 C;
  C = B;
  ASSERT_EQ(C, B);
}

TEST(EightBitRegisterType, ZeroFlag) {
  reg8 A;

  A.Z();
  ASSERT_EQ(A, 0b1000'0000);

  A.Z(false);
  ASSERT_EQ(A, 0b0000'0000);
}

TEST(EightBitRegisterType, NegationFlag) {
  reg8 A;

  A.N();
  ASSERT_EQ(A, 0b0100'0000);

  A.N(true);
  ASSERT_EQ(A, 0b0100'0000);

  A.N(false);
  ASSERT_EQ(A, 0b0000'0000);
}

TEST(EightBitRegisterType, BorrowingFlag) {
  reg8 A;

  A.H();
  ASSERT_EQ(A, 0b0010'0000);
}

TEST(EightBitRegisterType, CarryFlag) {
  reg8 A;

  A.C();
  ASSERT_EQ(A, 0b0001'0000);

  A.C(false);
  ASSERT_EQ(A, 0b0000'0000);
}

TEST(EightBitRegisterType, Allofthem) {
  reg8 A;

  A.Z();
  A.N();
  A.H();
  A.C();

  auto data = A.data();
  ASSERT_EQ(data, 0b1111'0000);
}

TEST(EightBitRegisterType, AssignReg8toAnIntermediateValue) {
  reg8 A;

  A = 0b0000'1010;
  ASSERT_EQ(A, 0b0000'1010);
}

TEST(EightBitRegisterType, AssignReg8toReg16) {
  uint16 val = 0b0110'0000'1010'0011;

  reg16 BC;
  BC = val;

  reg8 A;
  A = BC;
  ASSERT_EQ(A, 0b1010'0011);
}
