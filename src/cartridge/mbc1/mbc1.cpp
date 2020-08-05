#include "tmbl/config.h"
#include "tmbl/cartridge/mbc1/mbc1.h"

#include <vector>

namespace tmbl {
mbc1::mbc1(std::vector<byte> &rom, const std::size_t xram_size)
    : m_rom(rom), m_xram_size(xram_size) {
  if (m_xram_size > 8_KB) {
    m_xram_bank = xram_size / 8_KB;
  }

  m_rom_bank = rom.size() / 16_KB;
}

}
