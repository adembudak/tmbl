#include "tmbl/builtin/builtin.h"
#include "tmbl/config.h"
#include "tmbl/memory_map.h"

namespace tmbl {

byte builtin::readWRAM(const std::size_t index) const noexcept { return m_wram.at(index); }

void builtin::writeWRAM(const std::size_t index, const byte val) noexcept {
  m_wram.at(index) = val;
}

byte builtin::readHRAM(const std::size_t index) const noexcept { return m_hram.at(index); }

void builtin::writeHRAM(const std::size_t index, const byte val) noexcept {
  m_hram.at(index) = val;
}

byte builtin::readEcho(const std::size_t index) const noexcept { return readWRAM(index + 512_B); }

void builtin::writeEcho(const std::size_t index, const byte val) noexcept {
  writeWRAM(index + 512_B, val);
}

}
