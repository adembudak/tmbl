#include <tmbl/cartridge/type/mbc5.h>
#include <cstddef> // for std::size_t

namespace tmbl {

mbc5::mbc5(std::vector<byte> &&rom, std::size_t xram_size, const bool has_rumble)
    : m_rom(std::move(rom)) {
  if (xram_size > 0) {
    m_has_xram = true;
    m_xram.resize(xram_size);
  }

  if (has_rumble) {
    m_has_rumble = true;
  }
}

byte mbc5::read(const std::size_t index) const noexcept {
  if (index >= 0x0000 && index <= 0x3FFF) { // rom bank 0
    return m_rom.at(index);
  }

  else if (index >= 0x4000 && index <= 0x7FFF) { // rom bank 0-511
    const uint16 effective_rom_bank = (rom_bank_high << 8) | rom_bank_low;
    const std::size_t effective_index = 0x4000 * effective_rom_bank + (index - 0x4000);
    return m_rom.at(effective_index);
  }

  else if (index >= 0xA000 && index <= 0xBFFF) { // read xram
    if (xram_access_enabled) {
      const std::size_t effective_index = 0x2000 * ram_bank + (index - 0xA000);
      return m_xram.at(effective_index);
    } else {
      return 0xFF;
    }
  }
}

void mbc5::write(const std::size_t index, const byte val) noexcept {
  if (index >= 0x0000 && index <= 0x1FFF) { // enable ram
    xram_access_enabled = m_has_xram ? (val & 0b0000'1111) == 0b1010 : false;
  }

  else if (index >= 0x2000 && index <= 0x2FFF) { // select rom bank low
    rom_bank_low = val;
  }

  else if (index >= 0x3000 && index <= 0x3FFF) { // select rom bank high
    rom_bank_high = val & 0b1;
  }

  else if (index >= 0x4000 && index <= 0x5FFF) { // select ram bank or rumble motor
    if (m_has_rumble) {
      bool rumble_motor_activated = val & 0b0000'1000;
      /*
       *  while(rumble_motor_activated) {
       *     vibrate();
       *  }
       */
    } else {
      ram_bank = val;
    }
  }

  else if (index >= 0xA000 && index <= 0xBFFF) { // write xram
    if (xram_access_enabled) {
      const std::size_t effective_index = 0x2000 * ram_bank + (index - 0xA000);
      m_xram.at(effective_index) = val;
    } else {
      (void)val; // if xram not enabled, ignore write
    }
  }
}

std::size_t mbc5::rom_size() const noexcept { return m_rom.size(); }
std::size_t mbc5::rom_banks() const noexcept { return m_rom.size() / 0x4000; }

std::size_t mbc5::xram_size() const noexcept { return m_xram.size(); }
std::size_t mbc5::xram_banks() const noexcept { return m_xram.size() / 0x2000; }

bool mbc5::has_xram() const noexcept { return m_has_xram; }
bool mbc5::has_rumble() const noexcept { return m_has_rumble; }

}
