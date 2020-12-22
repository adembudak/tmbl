#include "tmbl/cpu/internals/r16.h"
#include "tmbl/cpu/internals/n16.h"
#include <gtest/gtest.h>

namespace {
using namespace tmbl;

TEST(r16, LoAndHi) {
  r16 HL;

  HL = 0xDEAD;
  EXPECT_TRUE(HL.hi() == 0xDE);
  EXPECT_TRUE(HL.lo() == 0xAD);
}

TEST(r16, AssignmentOperator) {
  r16 HL;

  n16 nn = 0xF00D;

  HL = nn;
  EXPECT_EQ(HL.value(), 0xF00D);
}

TEST(r16, IncrementDecrement) {
  r16 HL;

  HL++;
  EXPECT_EQ(1, HL.value());

  HL--;
  EXPECT_EQ(0, HL.value());

  HL = 0xFFFF;
  EXPECT_EQ(HL.value(), r16::max);
  HL++;
  EXPECT_EQ(HL.value(), r16::min);

  HL = 0;
  EXPECT_EQ(HL.value(), r16::min);
  HL--;
  EXPECT_EQ(HL.value(), r16::max);
}

TEST(r16, NonmemberOperatorPlus) {
  r16 HL, BC;

  HL = 0xFFFF;
  BC = 0x0001;

  r16 ret = HL + BC;
  EXPECT_EQ(ret.value(), 0x0000);

  HL = 0xC01A;
  BC = 0x1CE5;

  ret = HL + BC;
  EXPECT_EQ(ret.value(), 0xC01A + 0x1CE5);
}

TEST(r16, NonmemberComparisionOperator) {

  r16 HL;
  uint16 uu = 0;

  EXPECT_TRUE(HL == uu);

  uu = 1;
  EXPECT_FALSE(HL == uu);
}

}
