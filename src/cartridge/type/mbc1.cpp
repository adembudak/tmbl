#include "tmbl/config.h"
#include "tmbl/cartridge/type/mbc1.h"

#include <cstddef>

namespace tmbl {
mbc1::mbc1(std::vector<byte> &&rom, const std::size_t xram_size) {
  m_rom = std::move(rom);

  if (xram_size > 0) {
    m_has_xram = true;
    m_xram.resize(xram_size);
  }
}

byte mbc1::read(const std::size_t index) noexcept {
  if (index >= 0x0000 && index <= 0x3FFF) { // ROM Bank $00 (Read Only)
    byte effective_rom_bank_number = (mode == 0b0) ? 0 : (bank2 << 5);
    std::size_t effective_index = (effective_rom_bank_number << 14) | (index & 0b11'1111'1111'1111);

    return m_rom.at(effective_index);
  }

  else if (index >= 0x4000 && index <= 0x7FFF) { // Additional ROM Banks (Read Only)
    byte effective_rom_bank_number = (bank2 << 5) | bank1;
    std::size_t effective_index = (effective_rom_bank_number << 14) | (index & 0b11'1111'1111'1111);

    return m_rom.at(effective_index);
  }

  else if (index >= 0xA000 && index <= 0xBFFF) { // RAM Bank, if present ('Read'/Write)
    if (xram_access_enabled) {
      byte effective_xram_bank_number = (mode == 0b0) ? 0 : bank2;
      std::size_t effective_index =
          (effective_xram_bank_number << 13) | (index & 0b1'1111'1111'1111);
      return m_xram.at(effective_index);
    } else {
      return 0xFF; // return an undefined value
    }
  }

  else {
    // Bilemiyorum Altan
  }
}

void mbc1::write(const std::size_t index, const byte val) noexcept {
  if (index >= 0x0000 && index <= 0x1FFF) { // RAM Enable (Write Only), RAMG register
    xram_access_enabled = m_has_xram ? (val & 0b0000'1111) == 0b1010 : false;
  }

  else if (index >= 0x2000 && index <= 0x3FFF) { // ROM Bank Number (Write Only), BANK1 register
    bank1 = ((val & 0b0001'1111) == 0b00000) ? (val + 1) : (val & 0b0001'1111);
  }

  else if (index >= 0x4000 && index <= 0x5FFF) { // RAM Bank Num. or Upper Bits of ROM Bank Nu.(WO)
    bank2 = val & 0b11;                          // BANK2 register
  }

  else if (index >= 0x6000 && index <= 0x7FFF) { // ROM/RAM Mode Select (Write Only), Mode register
    mode = val & 0b1;
  }

  else if (index >= 0xA000 && index <= 0xBFFF) { // RAM Bank, if present (Read/'Write')
    if (xram_access_enabled) {
      byte effective_xram_bank_number = (mode == 0b0) ? 0 : bank2;
      std::size_t effective_index =
          (effective_xram_bank_number << 13) | (index & 0b1'1111'1111'1111);
      m_xram.at(effective_index) = val;
    } else {
      (void)val; // ignored, write has no effect
    }
  }

  else {
    // Bilemiyorum, bilemiyorum Altan...
  }
}

std::size_t mbc1::rom_size() const noexcept { return m_rom.size(); }
std::size_t mbc1::rom_banks() const noexcept { return m_rom.size() / 0x4000; }

std::size_t mbc1::xram_size() const noexcept { return m_xram.size(); }
std::size_t mbc1::xram_banks() const noexcept { return m_xram.size() / 0x2000; }

bool mbc1::has_xram() const noexcept { return m_has_xram; }
}
