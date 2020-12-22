#include "tmbl/cpu/internals/e8.h"
#include <gtest/gtest.h>

namespace {

using namespace tmbl;

TEST(e8, ctor) {
  e8 e = 127;

  EXPECT_EQ(e.value(), 127);

  e8 k = 128;
  EXPECT_EQ(k.value(), -128);

  e8 p = -10;
  EXPECT_EQ(p.value(), -10);
}

TEST(e8, nibbleGetters) {
  e8 e = 0x7F;

  EXPECT_EQ(e.hiNibble(), 0x07);
  EXPECT_EQ(e.loNibble(), 0x0F);
}

}

