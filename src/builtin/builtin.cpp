#include "tmbl/builtin/builtin.h"
#include "tmbl/config.h"
#include "tmbl/memory_map.h"

namespace tmbl {

byte builtin::readWRAM(const std::size_t index) { return m_wram.at(index); }
void builtin::writeWRAM(const std::size_t index, const byte val) { m_wram.at(index) = val; }

byte builtin::readHRAM(const std::size_t index) { return m_hram.at(index); }
void builtin::writeHRAM(const std::size_t index, const byte val) { m_hram.at(index) = val; }

byte builtin::readEcho(const std::size_t index) { return readWRAM(index - 8_KB); }
void builtin::writeEcho(const std::size_t index, const byte val) { writeWRAM(index - 8_KB, val); }

}

