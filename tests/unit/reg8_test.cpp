#include <gtest/gtest.h>
#include "tmbl/cpu/registers/reg8.h"

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

