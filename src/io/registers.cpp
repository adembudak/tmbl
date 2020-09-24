
#include "tmbl/config.h"
#include "tmbl/memory_map.h"
#include "tmbl/io/registers.h"

#include <array>
#include <cstddef>

namespace tmbl {
byte &registers::getAt(const std::size_t index) { return m_regs.at(index - memory::io); }

byte registers::read(const std::size_t index) { return m_regs.at(index); }
void registers::write(const std::size_t index, const byte val) {
  const std::size_t normalized_index = index - memory::io;
  m_regs.at(normalized_index) = val;
}

}

