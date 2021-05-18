#include "tmbl/config.h"
#include "tmbl/cartridge/type/mbc1.h"

#include <gtest/gtest.h>

#include <numeric> // for std::iota
#include <utility> // for std::move
#include <vector>
#include <cstddef> // for std::size_t

namespace {
using namespace tmbl;

class MBC1Test : public ::testing::Test {
public:
  void SetUp() override {
    std::vector<byte> fake_rom(fake_rom_size);

    std::iota(fake_rom.begin(), fake_rom.end(), 0);
    fake_rom.at(0x4000) = 99;                    // used at test ReadFromROMX
    fake_rom.at(rom_bank_size * 111) = 25;       // used at test ReadFromROMX
    fake_rom.at((rom_bank_size * 111) + 1) = 50; // used at test ReadFromROMX

    fake_rom.at((rom_bank_size * 1) + 0xFFF) = 88;    // ReadProblematicBanksROMX
    fake_rom.at((rom_bank_size * 0x21) + 0xFFF) = 77; // ReadProblematicBanksROMX
    fake_rom.at((rom_bank_size * 0x41) + 0xFFF) = 66; // ReadProblematicBanksROMX
    fake_rom.at((rom_bank_size * 0x61) + 0xFFF) = 55; // ReadProblematicBanksROMX

    mbc = mbc1(std::move(fake_rom), fake_xram_size);
  }

  const size_t xram_bank_size = 8_KB; // 0x2000
  const size_t rom_bank_size = 16_KB; // 0x4000

  const std::size_t fake_xram_size = 32_KB;
  const std::size_t fake_rom_size = 2_MB;

  mbc1 mbc;
};

TEST_F(MBC1Test, XRAMExistence) {
  EXPECT_TRUE(mbc.has_xram());
  EXPECT_EQ(mbc.xram_size(), fake_xram_size);
  EXPECT_EQ(mbc.xram_banks(), 4);
}

TEST_F(MBC1Test, ROMSize) {
  //
  EXPECT_TRUE(mbc.rom_size() == 2_MB);
}

TEST_F(MBC1Test, ROMBankNumber) {
  //
  EXPECT_EQ(mbc.rom_banks(), 125);
}

TEST_F(MBC1Test, ReadFromROM0) {
  mbc.write(0x6000, 0b00); // mode == 0b00, simple rom banking

  EXPECT_EQ(mbc.read(0x0000), 0);
  EXPECT_EQ(mbc.read(0x0001), 1);
  EXPECT_EQ(mbc.read(0x0002), 2);
  EXPECT_EQ(mbc.read(0x3FFF), 255);
}

TEST_F(MBC1Test, ReadROMX) {
  mbc.write(0x6000, 0b01); // mode == 0b01, advanced rom banking

  mbc.write(0x2000, 1); // bank1 == 0b0'0001;
  mbc.write(0x4000, 0); // bank2 == 0b00;
  // effective_rom_bank = (bank2 << 5) | bank1
  //                    = (0b00 << 5) | 0b0'0001;
  //                    = 0b0000'0001
  // effective_index  = (effective_rom_bank << 14) | (index & 0x3FFF);
  // let's say index is 0x4000
  // effective_index = 0x4000
  EXPECT_EQ(mbc.read(0x4000), 99);

  mbc.write(0x2000, 15); // bank1 == 0b0'1111;
  mbc.write(0x4000, 3);  // bank2 == 0b11;
  // effective_rom_bank = (0b11 << 5) | 0b0'1111;
  //                    = 0b0110'1111;
  // at index 0x4000
  // effective_index = (0b0110'1111 << 14) | (0x4000 & 0x3FFF);
  //                 = 1818624

  // sample values set on test class SetUp function
  EXPECT_EQ(mbc.read(0x4000), 25);
  EXPECT_EQ(mbc.read(0x4001), 50);
}

TEST_F(MBC1Test, ReadProblematicBanksROMX) {
  // 0x00
  mbc.write(0x6000, 0b01); // mode == 0b01, advanced rom banking

  mbc.write(0x2000, 0); // bank1 0b0000 should reset to 0b00001
  mbc.write(0x4000, 0); // bank2 0b00
  EXPECT_EQ(mbc.read(0x4FFF), 88);

  // 0x20
  mbc.write(0x2000, 0); // 0b00001
  mbc.write(0x4000, 1); // 0b01
  // effective_rom_bank =  (0b01 << 5) | 0b0000'0001
  //                    =  0b0010'0001
  //                    =  0x21
  EXPECT_EQ(mbc.read(0x4FFF), 77);

  // 0x40
  mbc.write(0x2000, 0); // 0b00001
  mbc.write(0x4000, 2); // 0b10
  // effective_rom_bank =  0b0100'0001
  //                    =  0x41
  EXPECT_EQ(mbc.read(0x4FFF), 66);

  // 0x60
  mbc.write(0x2000, 0); // 0b00001
  mbc.write(0x4000, 3); // 0b11

  EXPECT_EQ(mbc.read(0x4FFF), 55);
}

TEST_F(MBC1Test, ReadWriteRAM0) {
  mbc.write(0x0000, 0b1010); // enable ram

  mbc.write(0x6000, 0b00); // mode == 0b00, disable xram banking

  // only ram bank 0 available
  mbc.write(0xA000, 123);
  mbc.write(0xA001, 124);
  mbc.write(0xBFFF, 210);

  EXPECT_EQ(mbc.read(0xA000), 123);
  EXPECT_EQ(mbc.read(0xA001), 124);
  EXPECT_EQ(mbc.read(0xBFFF), 210);
}

TEST_F(MBC1Test, ReadWriteRAMX) {
  mbc.write(0x0000, 0b1010); // enable ram

  mbc.write(0x6000, 0b01); // mode == 1, enable xram banking
  mbc.write(0x4000, 0);    // bank2 == 0b00;

  mbc.write(0xA000, 123);
  mbc.write(0xA001, 124);
  mbc.write(0xBFFF, 210);

  EXPECT_EQ(mbc.read(0xA000), 123);
  EXPECT_EQ(mbc.read(0xA001), 124);
  EXPECT_EQ(mbc.read(0xBFFF), 210);

  mbc.write(0x4000, 1); // bank2 == 0b01;

  mbc.write(0xA000, 3);
  mbc.write(0xA001, 13);
  mbc.write(0xBFFF, 113);

  EXPECT_EQ(mbc.read(0xA000), 3);
  EXPECT_EQ(mbc.read(0xA001), 13);
  EXPECT_EQ(mbc.read(0xBFFF), 113);

  mbc.write(0x4000, 2); // bank2 == 0b10;

  mbc.write(0xA000, 0);
  mbc.write(0xA001, 77);
  mbc.write(0xBFFF, 44);

  EXPECT_EQ(mbc.read(0xA000), 0);
  EXPECT_EQ(mbc.read(0xA001), 77);
  EXPECT_EQ(mbc.read(0xBFFF), 44);

  mbc.write(0x4000, 3); // bank2 == 0b11;

  mbc.write(0xA000, 0);
  mbc.write(0xA001, 77);
  mbc.write(0xBFFF, 44);

  EXPECT_EQ(mbc.read(0xA000), 0);
  EXPECT_EQ(mbc.read(0xA001), 77);
  EXPECT_EQ(mbc.read(0xBFFF), 44);
}

}
