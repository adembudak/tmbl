#include "tmbl/config.h"
#include "tmbl/cartridge/rom/rom.h"
#include <utility>

namespace tmbl {

rom::rom(std::vector<char> &&rom_, int xram_size) : m_rom(std::move(rom_)) {
  if (xram_size > 0) {
    m_xram.resize(xram_size);
  }
}

byte rom::read_rom(const std::size_t index) { return m_rom.at(index); }
byte rom::read_xram(const std::size_t index) { return m_xram.size() ? m_xram.at(index) : 0xFF; }

void rom::write_xram(const std::size_t index, const byte val) {
  if (m_xram.size() > 0) {
    m_xram.at(index) = val;
  }
}

}
