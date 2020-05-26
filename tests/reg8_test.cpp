#include "tmbl/config.h"
#include <gtest/gtest.h>

TEST(RegisterTypes, reg8) {
  using namespace tmbl;
  reg8 A;
  ASSERT_TRUE(A == 0b0000'0000);
  ASSERT_EQ(A.data(), 0b0000'0000);

  reg8 B;

  A = B;
  ASSERT_EQ(A, B);
  reg8 C = A;
  ASSERT_EQ(A, C);
}
