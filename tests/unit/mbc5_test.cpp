#include "tmbl/config.h"
#include "tmbl/cartridge/type/mbc5.h"

#include <gtest/gtest.h>

#include <numeric>
#include <vector>

namespace {
using namespace tmbl;

class MBC5Test : public ::testing::Test {
public:
  void SetUp() override {
    std::vector<byte> fake_rom(m_fake_rom_size);
    std::iota(fake_rom.begin(), fake_rom.end(), 0);

    fake_rom.at(m_rom_bank_size * 0) = 0;
    fake_rom.at(m_rom_bank_size * 1) = 4;
    fake_rom.at(m_rom_bank_size * 255) = 8;
    fake_rom.at(m_rom_bank_size * 511) = 16;
    fake_rom.at(m_rom_bank_size * 256) = 32;

    mbc = mbc5(std::move(fake_rom), m_fake_xram_size, /*has_rumble=*/false);
  }

  const size_t m_xram_bank_size = 8_KB; // 0x2000
  const size_t m_rom_bank_size = 16_KB; // 0x4000

  const std::size_t m_fake_xram_size = 128_KB;
  const std::size_t m_fake_rom_size = 8_MB;

  mbc5 mbc;
};

TEST_F(MBC5Test, ReadROM0) {
  EXPECT_EQ(mbc.read(0), 0);
  EXPECT_EQ(mbc.read(1), 1);
  EXPECT_EQ(mbc.read(255), 255);

  // rom_bank_high == 0 and rom_bank_low == 0 by default
  EXPECT_EQ(mbc.read(0x4000), 0);
  EXPECT_EQ(mbc.read(0x4001), 1);
  EXPECT_EQ(mbc.read(0x40FF), 255);
}

TEST_F(MBC5Test, ReadWriteROMX) {
  // effective rom bank is calculated with: (hi << 8) | lo
  mbc.write(0x2000, 0); // select rom bank low
  mbc.write(0x3000, 0); // select rom bank high
  EXPECT_EQ(mbc.read(0x4000), 0);

  mbc.write(0x2000, 1);
  mbc.write(0x3000, 0);
  EXPECT_EQ(mbc.read(0x4000), 4);

  mbc.write(0x2000, 255);
  mbc.write(0x3000, 0);
  EXPECT_EQ(mbc.read(0x4000), 8);

  mbc.write(0x2000, 255);
  mbc.write(0x3000, 1);
  EXPECT_EQ(mbc.read(0x4000), 16);

  mbc.write(0x2000, 0);
  mbc.write(0x3000, 1);
  EXPECT_EQ(mbc.read(0x4000), 32);
}

TEST_F(MBC5Test, ReadXRAM) {
  mbc.write(0x0000, 0b1010); // eneble xram

  mbc.write(0xA000, 99); // xram_bank 0 by default
  EXPECT_EQ(mbc.read(0xA000), 99);

  mbc.write(0x4000, 0); // set xram bank 0, explicitly
  EXPECT_EQ(mbc.read(0xA000), 99);

  mbc.write(0x4000, 1);             // select xram bank 1
  mbc.write(0xA000, 123);           // write something to that bank
  EXPECT_EQ(mbc.read(0xA000), 123); // read and compare

  mbc.write(0x5FFF, 15); // select 16th xram bank
  mbc.write(0xBFFF, 255);
  EXPECT_EQ(mbc.read(0xBFFF), 255);
}

TEST_F(MBC5Test, write) { EXPECT_TRUE(true); }

TEST_F(MBC5Test, ROMSize) { EXPECT_EQ(mbc.rom_size(), 8_MB); }
TEST_F(MBC5Test, ROMBanks) { EXPECT_EQ(mbc.rom_banks(), 512); }

TEST_F(MBC5Test, XRAMSize) { EXPECT_EQ(mbc.xram_size(), 128_KB); }
TEST_F(MBC5Test, XRAMBanks) { EXPECT_EQ(mbc.xram_banks(), 16); }

TEST_F(MBC5Test, XRAMExistence) { EXPECT_TRUE(mbc.has_xram()); }
TEST_F(MBC5Test, RumbleExistence) { EXPECT_FALSE(mbc.has_rumble()); }

}
