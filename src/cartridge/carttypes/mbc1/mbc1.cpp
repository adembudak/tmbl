#include "tmbl/config.h"
#include "tmbl/cartridge/carttypes/mbc1/mbc1.h"

#include <cstddef>
#include <vector>
#include <algorithm>

namespace tmbl {

mbc1::mbc1(std::vector<char> &&rom, std::size_t xram_size) : m_rom(std::move(rom)) {
  if (xram_size != 0) {
    ram_available = true;
    m_xram.resize(xram_size);
    if (xram_size > 8_KB) {
      ram_banking_available = true;
    }
  }

  if (auto rom_size = m_rom.size(); rom_size > 32_KB) {
    rom_banking_available = true;
    if (rom_size == 256_KB) { // adjust lower rom mask, and upper rom mask shift (5 by default)
      lower_rom_mask >>= 1;
      lower_rom_mask_countl_one -= 1;
    } else if (rom_size == 128_KB) {
      lower_rom_mask >>= 2;
      lower_rom_mask_countl_one -= 2;
    } else if (rom_size == 64_KB) {
      lower_rom_mask >>= 3;
      lower_rom_mask_countl_one -= 3;
    } else {
    }
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
      ram_bank = ram_banking_available ? val & ram_mask : 0;
    }
  }

  else if (index >= 0x6000 && index <= 0x7FFF) {          // register 4, decide banking mode
    if (m_rom.size() > 512_KB || ram_banking_available) { // mode0: rom banking, mode1: ram banking
      banking_mode = (val & 0b0000'0001) ? mode::ram_banking : mode::rom_banking;
    }
  }

  else if (index >= 0xA000 && index <= 0xBFFF) {
    if (ram_available && ram_access_enabled) {
      m_xram.at((ram_bank * ram_bank_width) + index) = val;
    }
  }
}

byte mbc1::read(const std::size_t index) {
  if (index >= 0x0000 && index <= 0x3FFF) {
    effective_rom_bank =
        (banking_mode == mode::rom_banking) ? 0 : (upper_rom_bank << lower_rom_mask_countl_one);
    return m_rom.at((effective_rom_bank * rom_bank_width) + index);
  }

  else if (index >= 0x4000 && index <= 0x7FFF) {
    effective_rom_bank = (upper_rom_bank << lower_rom_mask_countl_one) | lower_rom_bank;
    return m_rom.at((effective_rom_bank * rom_bank_width) + index);
  }

  else if (index >= 0xA000 && index <= 0xBFFF) {
    return (ram_available && ram_access_enabled) ? m_xram.at((ram_bank * ram_bank_width) + index)
                                                 : 0xFF;
  }
}

}

