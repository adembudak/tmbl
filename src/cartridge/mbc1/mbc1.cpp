#include "tmbl/config.h"
#include "tmbl/cartridge/mbc1/mbc1.h"

#include <cstddef>
#include <vector>
#include <algorithm>

namespace tmbl {



mbc1::mbc1(std::vector<byte> &rom, std::size_t xram_size) : m_rom(rom) {
  if (xram_size != 0) {
    m_xram.resize(xram_size);
  }
}

void mbc1::write(const std::size_t index, const byte val) {

  if (index >= 0x0000 && index <= 0x1FFF) { // ram chip select
    if ((val & 0b0000'1111) == 0x0A) {
      ram_access_enabled = true;
    } else {
      ram_access_enabled = false;
    }
  }

  else if (index >= 0x2000 && index <= 0x3FFF) { // lower rom bank select
    if (uint8 ret = val & 0x10; ret == 0 || ret == 20 || ret == 40 || ret == 60) {
      lower_rom_bank = val + 1;
      return;
    }

    if (val >= 0x01 && val <= 0x1F) {
      lower_rom_bank = val & lower_rom_mask;
      if (lower_rom_bank == 0 || lower_rom_bank == 20 || lower_rom_bank == 40 ||
          lower_rom_bank == 60) {
        ++lower_rom_bank;
      }
    }
  }

  else if (index >= 0x4000 && index <= 0x5FFF) { // decide upper rom bank or ram bank number
    if (banking_mode == mode::rom_banking) {     // rom banking mode
      if (m_rom.size() >= 512_KB) {
        upper_rom_bank = val & upper_rom_mask;
        effective_rom_bank = (upper_rom_bank << 5) | lower_rom_bank;
      } else {
        effective_rom_bank = lower_rom_bank;
      }
    } else { // ram banking mode
      if (m_xram.size() == 32_KB) {
        ram_bank = val & ram_mask;
      }
    }
  }

  else if (index >= 0x6000 && index <= 0x7FFF) {
    if (m_rom.size() > 512_KB || m_xram.size() > 8_KB) {
      if ((val & 0b0000'0001) == 0) {
        banking_mode = mode::rom_banking;
      } else {
        banking_mode = mode::ram_banking;
      }
    }
  }
}
}
