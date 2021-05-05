#include "tmbl/config.h"
#include "tmbl/cartridge/type/rom.h"

#include <gtest/gtest.h>

#include <numeric>
#include <vector>
#include <utility>

namespace {
using namespace tmbl;

class ROMTest : public ::testing::Test {
protected:
  void SetUp() override {
    std::vector<byte> fake_rom(32_KB);
    std::iota(fake_rom.begin(), fake_rom.end(), 0);
    m_rom = rom(std::move(fake_rom));
  }

  rom m_rom;
};

TEST_F(ROMTest, ReadROM) {
  EXPECT_EQ(m_rom.read(0), 0);
  EXPECT_EQ(m_rom.read(1), 1);
  EXPECT_EQ(m_rom.read(32767), 255);
}

TEST_F(ROMTest, WriteROM) {
  m_rom.write(0, 123);
  m_rom.write(1, 12);
  m_rom.write(32767, 1);

  EXPECT_NE(m_rom.read(0), 123);
  EXPECT_NE(m_rom.read(1), 12);
  EXPECT_NE(m_rom.read(32767), 1);
}

TEST_F(ROMTest, ReadWriteRam) {
  m_rom.write_xram(0, 123);
  m_rom.write_xram(8191, 12);

  // has no ram, writes should has no effect
  EXPECT_NE(m_rom.read_xram(0), 123);
  EXPECT_NE(m_rom.read_xram(8191), 12);
}

class ROMwithXRAMTest : public ::testing::Test {
protected:
  void SetUp() override {
    std::vector<byte> fake_rom(32_KB);
    std::iota(fake_rom.begin(), fake_rom.end(), 0);

    const std::size_t ram_size = 8_KB;

    m_rom = rom(std::move(fake_rom), ram_size);
  }

  rom m_rom;
};

TEST_F(ROMwithXRAMTest, ReadWriteRam) {
  m_rom.write_xram(0, 123);
  m_rom.write_xram(8191, 12);

  EXPECT_EQ(m_rom.read_xram(0), 123);
  EXPECT_EQ(m_rom.read_xram(8191), 12);
}

}
