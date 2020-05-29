#include "tmbl/cpu/registers/reg8.h"
#include "tmbl/cpu/registers/reg16.h"
#include <tmbl/config.h>

#include <gtest/gtest.h>

using namespace tmbl;
using namespace tmbl::cpu;

TEST(SixteenBitRegisterType, Construction) {
  reg16 HL;
  reg16 BC = HL;

  ASSERT_EQ(BC, HL);

  reg16 DE;
  DE = BC;
  ASSERT_EQ(DE, BC);
}

TEST(SixteenBitRegisterType, AssignmentOperatorOverloads) {
  // Assign reg8
  reg8 A;
  A = 0b1011'0001;

  reg16 HL;
  HL = A;

  ASSERT_EQ(HL, A);

  // Assign uint16 intermediate value
  uint16 val = 0b1111'0000'1111'0000;
  HL = val;

  ASSERT_EQ(HL.data(), val);

  // increase by one, for PC register
  HL += 1;

  ASSERT_EQ(HL.data(), val + 1);
}

TEST(SixteenBitRegisterType, membersHigh) {
  reg16 HL;
  uint16 HL_val = 0b1111'1010'0101'0000;
  HL = HL_val;

  reg8 A;
  uint8 A_val = 0b1100'0011;
  A = A_val;

  HL.hi(A);

  ASSERT_EQ(HL.data(), 0b1100'0011'0101'0000);

  reg8 F = HL.hi();
  ASSERT_EQ(F.data(), 0b1100'0011);
  ASSERT_EQ(F, A);
}

TEST(SixteenBitRegisterType, membersLow) {
  reg16 HL;
  uint16 HL_val = 0b1111'1010'0101'0000;
  HL = HL_val;

  reg8 A;
  uint8 A_val = 0b1100'0011;
  A = A_val;

  HL.lo(A);

  ASSERT_EQ(HL.data(), 0b1111'1010'1100'0011);
  reg8 F = HL.lo();
  ASSERT_EQ(F.data(), 0b1100'0011);
  ASSERT_EQ(F, A);
}

