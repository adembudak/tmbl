#include "tmbl/builtin/builtin.h"
#include "tmbl/config.h"
#include "tmbl/memory_map.h"

namespace tmbl {

byte builtin::readWRAM(const std::size_t index) { return m_wram.at(index - memory::wram); }

void builtin::writeWRAM(const std::size_t index, const byte val) {
  m_wram.at(index - memory::wram) = val;
}

void builtin::writeHRAM(const std::size_t index, const byte val) {
  m_hram.at(index - memory::hram) = val;
}

byte builtin::readHRAM(const std::size_t index) { return m_hram.at(index - memory::hram); }
}

