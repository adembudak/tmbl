#include "tmbl/cpu/internals/n8.h"
#include <gtest/gtest.h>

namespace {

using namespace tmbl;

TEST(n8, ctor) {
  n8 n = 0xFB;
  EXPECT_EQ(n.value(), 0xFB);
}

TEST(n8, nibbleGetters) {
  n8 n = 0x34;
  EXPECT_EQ(n.hiNibble(), 0x03);
  EXPECT_EQ(n.loNibble(), 0x04);
}

TEST(n8, NonmemberComparisionOperators) {
  n8 n = 0x1D;
  n8 k = 0xEA;

  EXPECT_FALSE(n == k);
  EXPECT_TRUE(n != k);

  EXPECT_TRUE(n < k);
  EXPECT_FALSE(n > k);

  EXPECT_TRUE(n <= k);
  EXPECT_FALSE(n >= k);
}

}

