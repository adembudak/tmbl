#include "tmbl/config.h"
#include "tmbl/cartridge/type/mbc1.h"

#include <gtest/gtest.h>

#include <numeric>
#include <utility>
#include <vector>
#include <cstddef>

namespace {
using namespace tmbl;

class MBC1Test : public ::testing::Test {
public:
  void SetUp() override {
    fake_rom.resize(2_MB);
    std::iota(fake_rom.begin(), fake_rom.end(), 0);
    fake_rom.at(0x94000) = 123;
    fake_rom.at(0x94000 + 1) = 77;

    mbc = mbc1(std::move(fake_rom), fake_xram_size);
  }

  std::vector<byte> fake_rom;
  const std::size_t fake_xram_size = 32_KB;
  mbc1 mbc;
};

TEST_F(MBC1Test, xram) {
  EXPECT_TRUE(mbc.has_xram());
  EXPECT_EQ(mbc.xram_size(), fake_xram_size);
  EXPECT_EQ(mbc.xram_banks(), 4);
}

TEST_F(MBC1Test, rom) {
  EXPECT_TRUE(mbc.rom_size() == 2_MB);
  EXPECT_EQ(mbc.rom_banks(), 125);
}

TEST_F(MBC1Test, read_write_xram) {
  mbc.write(0x0000, 0b1111'1010); // ramg, enable ram access
  mbc.write(0x4000, 0b11);        // bank2 is 3
  mbc.write(0x6000, 1);           // mode is 1

  mbc.write(0xA000, 123);
  EXPECT_EQ(mbc.read(0xA000), 123);

  mbc.write(0x0000, 0b1111'1111); // disable ram access
  mbc.write(0xA000, 0xAA);        // write has no effect
  EXPECT_FALSE(mbc.read(0xA000) == 0xAA);
}

TEST_F(MBC1Test, read_rom) {
  mbc.write(0x2000, 5); // bank1 is 5
  mbc.write(0x4000, 1); // bank2 is 0

  // (bank2 << 5) | (bank1)
  // now effective rom bank should be 37 == 0b1'0101,
  // effective index should be 37*0x4000 == 0x94000 == 606208

  EXPECT_EQ(mbc.read(0x4000), 123);
  EXPECT_EQ(mbc.read(0x4000 + 1), 77);
}

}
