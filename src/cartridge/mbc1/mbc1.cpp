#include "tmbl/config.h"
#include "tmbl/cartridge/mbc1/mbc1.h"

#include <cstddef>
#include <vector>

namespace tmbl {
mbc1::mbc1(std::vector<byte> &rom, const std::size_t xram_size)
    : m_rom(rom), m_xram_size(xram_size) {
  if (m_xram_size > 8_KB) {
    m_xram_bank = xram_size / 8_KB;
  }

  m_rom_bank = rom.size() / 16_KB;
}

std::size_t mbc1::bankNumber() const noexcept { return m_rom_bank; }

byte mbc1::read_rom(const std::size_t index) { return m_rom.at(index); }
byte mbc1::read_xram(const std::size_t index) { return m_xram.at(index); }
void mbc1::write_xram(const std::size_t index, const byte val) { m_xram.at(index) = val; }

}
