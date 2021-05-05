#include "tmbl/config.h"
#include "tmbl/cartridge/type/mbc2.h"

#include <gtest/gtest.h>

#include <vector>
#include <numeric>

namespace {
using namespace tmbl;

class MBC2Test : public ::testing::Test {
protected:
  void SetUp() override {
    std::vector<byte> fake_ROM(256_KB);

    std::iota(fake_ROM.begin(), fake_ROM.end(), 0);

    fake_ROM.at(0) = 10;
    fake_ROM.at(1) = 20;
    fake_ROM.at(2) = 30;

    fake_ROM.at(0x4000) = 99;
    fake_ROM.at(0x4001) = 88;
    fake_ROM.at(0x4002) = 77;

    fake_ROM.at(0x4000 * 15) = 111;
    fake_ROM.at((0x4000 * 15) + 1) = 222;

    mbc = mbc2(std::move(fake_ROM));
  }

  mbc2 mbc;
};

TEST_F(MBC2Test, ReadROM0) {
  EXPECT_EQ(mbc.read(0), 10);
  EXPECT_EQ(mbc.read(1), 20);
  EXPECT_EQ(mbc.read(2), 30);
}

TEST_F(MBC2Test, ReadROMX) {
  mbc.write(0x0000, 0b1000'0001); // romb should be 1

  EXPECT_EQ(mbc.read(0x4000), 99);
  EXPECT_EQ(mbc.read(0x4001), 88);
  EXPECT_EQ(mbc.read(0x4002), 77);

  mbc.write(0x0000, 0b1000'0000); // again, romb should be 1
  EXPECT_EQ(mbc.read(0x4000), 99);
  EXPECT_EQ(mbc.read(0x4001), 88);
  EXPECT_EQ(mbc.read(0x4002), 77);

  mbc.write(0x0000, 0b1000'1111); // romb should be 15
  EXPECT_NE(mbc.read(0x4000), 99);
  EXPECT_NE(mbc.read(0x4001), 88);
  EXPECT_NE(mbc.read(0x4002), 77);

  EXPECT_EQ(mbc.read(0x4000), 111);
  EXPECT_EQ(mbc.read(0x4001), 222);
}

TEST_F(MBC2Test, ReadWriteRAM) {
  mbc.write(0x0000, 0b0000'1010); // enable ram access, ramg register

  mbc.write(0xA000, 0b1111'1100);
  EXPECT_EQ(mbc.read(0xA000), 0b0000'1100); // should discard upper nibble

  EXPECT_EQ(mbc.read(0xA200), 0b0000'1100); // should wrap every 512 bytes
  EXPECT_EQ(mbc.read(0xA400), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xA600), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xA800), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xAA00), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xAC00), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xAE00), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xB000), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xB200), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xB400), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xB600), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xB800), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xBA00), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xBC00), 0b0000'1100);
  EXPECT_EQ(mbc.read(0xBE00), 0b0000'1100);

  mbc.write(0x0000, 0b0000'0110); // disable ram access

  mbc.write(0xA000, 0b1111'1001); // write should be discarded
  EXPECT_NE(mbc.read(0xA000), 0b1111'1001);
}

}
