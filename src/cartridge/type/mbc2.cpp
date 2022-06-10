#include "tmbl/config.h"
#include "tmbl/utils/utils.h"
#include <tmbl/cartridge/type/mbc2.h>

namespace tmbl {

mbc2::mbc2(std::vector<byte> &&rom) : m_rom(std::move(rom)) {
  m_xram.resize(512_B); // mbc2 type cartridges has only 512x4 bit of xram
}

byte mbc2::read(const std::size_t index) const noexcept { // ROM Bank 0 (Read only)
  if (index >= 0x0000 && index <= 0x3FFF) {               // bank 0
    return m_rom[index];
  }

  else if (index >= 0x4000 && index <= 0x7FFF) { // ROM Bank 1-15
    std::size_t effective_index = (romb << 14) | (index & 0b11'1111'1111'1111);

    return m_rom[effective_index];
  }

  else if (index >= 0xA000 && index <= 0xBFFF) { // RAM ('read'/write)
    if (xram_access_enabled) {
      const byte data = m_xram[index % 512] & 0b0000'1111;
      return data;
    } else {
      return utils::randomByte();
    }
  }

  else {
    // things
  }
}

void mbc2::write(const std::size_t index, const byte val) noexcept {
  if (index >= 0x0000 && index <= 0x3FFF) { // ramg and romb registers
    if (val & 0b1000'0000) {                // romb
      romb = ((val & 0b0000'1111) == 0) ? 1 : (val & 0b0000'1111);
    } else { // ramg
      xram_access_enabled = (val & 0b0000'1111) == 0b1010;
    }
  }

  else if (index >= 0xA000 && index <= 0xBFFF) { // RAM (read/'write')
    if (xram_access_enabled) {
      const byte data = val & 0b0000'1111; // mbc2 ram has only 4 bit of datum
      m_xram[index % 512] = data;
    } else {
      (void)val;
    }
  }

  else {
  }
}

}
