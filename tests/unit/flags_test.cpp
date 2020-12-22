#include "tmbl/cpu/internals/flags.h"
#include <gtest/gtest.h>

namespace {

using namespace tmbl;

TEST(flags, DefaultValues) {
  flags F;
  EXPECT_TRUE(F.value() == 0);
}

TEST(flags, SettersAndGetters) {
  flags F;

  // zero flag
  EXPECT_EQ(F.z(), reset);

  F.z(set);
  EXPECT_EQ(F.z(), set);

  F.z(reset);
  EXPECT_EQ(F.z(), reset);

  // negation flag
  EXPECT_EQ(F.n(), reset);
  F.n(set);
  EXPECT_EQ(F.n(), set);

  F.n(reset);
  EXPECT_EQ(F.n(), reset);

  // half carry flag
  EXPECT_EQ(F.h(), reset);
  F.h(set);
  EXPECT_EQ(F.h(), set);

  F.h(reset);
  EXPECT_EQ(F.h(), reset);

  // carry flag
  EXPECT_EQ(F.c(), reset);
  F.c(set);
  EXPECT_EQ(F.c(), set);

  F.c(reset);
  EXPECT_EQ(F.c(), reset);
}

TEST(flags, AssignValueWithByte) {
  flags F;
  F = 0xA0; // 0b1010'0000

  EXPECT_TRUE(F.z() == set);
  EXPECT_TRUE(F.n() == reset);
  EXPECT_TRUE(F.h() == set);
  EXPECT_TRUE(F.c() == reset);
}
}
