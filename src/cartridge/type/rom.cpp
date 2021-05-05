#include "tmbl/config.h"
#include "tmbl/cartridge/type/rom.h"

#include <utility>
#include <cstddef>

namespace tmbl {

rom::rom(std::vector<byte> &&rom_, int xram_size) : m_rom(std::move(rom_)) {
  if (xram_size > 0) {
    has_xram = true;
    m_xram.resize(xram_size);
  }
}

byte rom::read(const std::size_t index) noexcept { return m_rom.at(index); }

void rom::write(const std::size_t index, const byte val) noexcept {
  (void)index;
  (void)val;
}

byte rom::read_xram(const std::size_t index) noexcept {
  return has_xram ? m_xram.at(index) : std::rand();
}

void rom::write_xram(const std::size_t index, const byte val) noexcept {
  if (has_xram) {
    m_xram.at(index) = val;
  } else {
    (void)val;
  }
}

}
