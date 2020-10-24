#include "tmbl/config.h"
#include "tmbl/cartridge/mbc1/mbc1.h"

#include <cstddef>
#include <vector>
#include <algorithm>

#include <iostream>

namespace tmbl {

mbc1::mbc1(std::vector<char> &rom, std::size_t xram_size) : m_rom(rom) {
  if (xram_size != 0) {
    m_xram.resize(xram_size);
  }
}

void mbc1::write(const std::size_t index, const byte val) {

  if (index >= 0x0000 && index <= 0x1FFF) { // ram chip select
    ram_access_enabled = ((val & 0b0000'1111) == 0x0A);
  }

  else if (index >= 0x2000 && index <= 0x3FFF) { // lower rom bank select
    if ((val & 0x1F) == 0) {
      lower_rom_bank = val + 1;
      return;
    }

    lower_rom_bank = val & lower_rom_mask;
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
      banking_mode = (val & 0b0000'0001) ? mode::ram_banking : mode::rom_banking;
    }
  }
}

  byte mbc1::read(const std::size_t index) {
     std::cout << "byte mbc1::read(const std::size_t index)\n";
     return 0xFF;
  }

}

