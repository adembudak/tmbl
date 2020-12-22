#include "tmbl/cpu/internals/n16.h"
#include <gtest/gtest.h>

namespace {

using namespace tmbl;

TEST(n16, ctor) {
  n16 nn = 0x51DE;
  EXPECT_EQ(nn.value(), 0x51DE);
}

TEST(n16, hiloGetters) {
  n16 nn = 0x5E1F;

  EXPECT_EQ(nn.hi(), 0x5E);
  EXPECT_EQ(nn.lo(), 0x1F);
}

}

