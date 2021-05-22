
#include "tmbl/config.h"
#include "tmbl/memory_map.h"
#include "tmbl/io/registers.h"

#include <array>
#include <cstddef>

namespace tmbl {
byte &registers::getAt(const std::size_t index) { return m_regs.at(index - memory::io); }

byte registers::read(const std::size_t index) const noexcept { return m_regs.at(index); }

void registers::write(const std::size_t index, const byte val) noexcept {
  if (index == 0xFF04) { // Writes to DIV, resets it to zero
    m_regs.at(index) = 0;
  } else {
    m_regs.at(index) = val;
  }
}

}
