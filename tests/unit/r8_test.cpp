#include "tmbl/cpu/internals/r8.h"
#include "tmbl/cpu/internals/n8.h"
#include <gtest/gtest.h>

namespace {

TEST(r8, defaultstate) {
  tmbl::r8 A;
  EXPECT_EQ(0, A.value());
}

TEST(r8, assignmentOperators) {
  tmbl::r8 A, F;
  A = F;
  EXPECT_EQ(A.value(), F.value());

  A = 255;
  EXPECT_EQ(255, A.value());

  tmbl::n8 n(21);
  A = n;
  EXPECT_EQ(21, A.value());

  A++;
  EXPECT_EQ(22, A.value());
  A--;
  EXPECT_EQ(21, A.value());

  ++A;
  EXPECT_EQ(22, A.value());
  --A;
  EXPECT_EQ(21, A.value());
}

TEST(r8, OperatorPlusPlusAndMinusMinus) {
  tmbl::r8 A;

  A++;
  EXPECT_EQ(1, A.value());
  A--;
  EXPECT_EQ(0, A.value());

  ++A;
  EXPECT_EQ(1, A.value());
  --A;
  EXPECT_EQ(0, A.value());

  --A; // r8 holds uint8_t type
  EXPECT_EQ(255, A.value());

  tmbl::r8 F;
  F = A++; // postincrement
  EXPECT_EQ(255, F.value());
  EXPECT_EQ(0, A.value());

  F = ++A; // preincrement
  EXPECT_EQ(1, F.value());
  EXPECT_EQ(1, A.value());

  F = A--;
  EXPECT_EQ(1, F.value());
  EXPECT_EQ(0, A.value());

  F = --A;
  EXPECT_EQ(255, F.value());
  EXPECT_EQ(255, A.value());
}

TEST(r8, NonmemberOperatorPlus) {
  tmbl::r8 A;
  tmbl::r8 F;

  A = 10;
  F = 20;

  tmbl::r8 result = A + F;
  EXPECT_EQ(result.value(), 30);

  A = 255;
  F = 1;
  result = A + F;
  EXPECT_EQ(result.value(), 0);

  A = 0;
  tmbl::uint8 val = 1;
  tmbl::uint8 ret = A + val;
  EXPECT_EQ(val, 1);
}

TEST(r8, NonmemberOperatorMinus) {
  tmbl::r8 A;
  tmbl::r8 F;

  A = 10;
  F = 10;

  tmbl::r8 result = A - F;
  EXPECT_EQ(result.value(), 0);

  A = 0;
  tmbl::uint8 val = 100;
  tmbl::uint8 ret = A - val;
  EXPECT_EQ(ret, 156);
}

TEST(r8, Comparision) {
  tmbl::r8 x, y;

  EXPECT_TRUE(x == y);
  EXPECT_TRUE(x <= y);
  EXPECT_TRUE(x >= y);

  x = 1;
  EXPECT_TRUE(x > y);
  EXPECT_FALSE(x < y);
  EXPECT_FALSE(x == y);

  y = 2;
  EXPECT_TRUE(x < y);
  EXPECT_FALSE(x > y);
  EXPECT_FALSE(x == y);

  tmbl::uint8 val = 2;
  EXPECT_TRUE(y == val);
}

TEST(r8, CompareWithUint8) {
  tmbl::r8 r;
  tmbl::uint8 u;

  r = 1;
  u = 1;

  EXPECT_TRUE(r == u);
  EXPECT_TRUE(r <= u);
  EXPECT_TRUE(r >= u);

  u = 2;
  EXPECT_TRUE(r != u);
  EXPECT_TRUE(r < u);

  r = 3;
  EXPECT_TRUE(r > u);
}

TEST(r8, NibbleGetters) {
  tmbl::r8 x;
  x = 0xFA;

  EXPECT_EQ(x.loNibble(), 0x0A);
  EXPECT_EQ(x.hiNibble(), 0x0F);
}

}
