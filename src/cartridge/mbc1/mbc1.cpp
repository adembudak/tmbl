#include "tmbl/config.h"
#include "tmbl/cartridge/mbc1/mbc1.h"

#include <cstddef>
#include <vector>
#include <algorithm>

#include <iostream>

#if defined(_MSC_VER)
#include <ciso646>
#endif

namespace tmbl {

mbc1::mbc1(std::vector<char> &rom, std::size_t xram_size) : m_rom(rom) {
  if (xram_size != 0) {
    m_xram.resize(xram_size);
  }
}

void mbc1::write(const std::size_t index, const byte val) {

  if (index >= 0x0000 && index <= 0x1FFF) { // register 1, ram chip select
    ram_access_enabled = ((val & 0b0000'1111) == 0x0A);
  }

  else if (index >= 0x2000 && index <= 0x3FFF) { // register 2, lower rom bank select
    if ((val & 0x1F) == 0) {
      lower_rom_bank = val + 1;
      return;
    }

    lower_rom_bank = val & lower_rom_mask;
  }

  else if (index >= 0x4000 && index <= 0x5FFF) { // register 3, decide upper rom/ram bank number
    if (banking_mode == mode::rom_banking) {     // rom banking mode
      upper_rom_mask = (m_rom.size() >= 512_KB) ? val & upper_rom_mask : 0;
      ram_bank = 0;
    } else { // ram banking mode
      ram_bank = (m_xram.size() == 32_KB) ? val & ram_mask : 0;
    }
  }

  else if (index >= 0x6000 && index <= 0x7FFF) {         // register 4, decide banking mode
    if (m_rom.size() > 512_KB || m_xram.size() > 8_KB) { // mode 0: rom banking, mode 1: ram banking
      banking_mode = (val & 0b0000'0001) ? mode::ram_banking : mode::rom_banking;
    }
  }

  else if (index >= 0xA000 && index <= 0xBFFF) {
    if (ram_access_enabled and m_xram.size() != 0) {
      m_xram.at((ram_bank * ram_bank_width) + index) = val;
    }
  } else {
    // Bilemiyorum Altan...
  }
}

byte mbc1::read(const std::size_t index) {
  if (index >= 0x0000 && index <= 0x3FFF) {
    effective_rom_bank = (banking_mode == mode::rom_banking) ? 0 : (upper_rom_bank << 5);
    return m_rom.at((effective_rom_bank * rom_bank_width) + index);
  }

  else if (index >= 0x4000 && index <= 0x7FFF) {
    effective_rom_bank = (upper_rom_bank << 5) | lower_rom_bank;
    return m_rom.at((effective_rom_bank * rom_bank_width) + index);
  }

  else if (index >= 0xA000 && index <= 0xBFFF) {
    if (ram_access_enabled and m_xram.size() != 0) {
      return m_xram.at((ram_bank * ram_bank_width) + index);
    }
    return 0xFF;
  } else {
    // Bilemiyorum, bilemiyorum Altan...
  }
}

}

