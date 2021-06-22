
#include "tmbl/config.h"
#include "tmbl/memory_map.h"
#include "tmbl/io/registers.h"

#include <array>
#include <cstddef>

namespace tmbl {
byte &registers::getAt(const std::size_t index) { return m_regs.at(index - memory::io); }

byte registers::read(const std::size_t index) const noexcept {
  // HDMA-1/2/3/4 registers returns 0xff on read
  if (index == 0xFF51 || index == 0xFF52 || index == 0xFF53 || index == 0xFF54) {
    return 0xFF;
  } else {
    return m_regs.at(index);
  }
}

void registers::write(const std::size_t index, const byte val) noexcept {
  if (index == 0xFF04) { // DIV register reset to zero on write
    m_regs.at(index) = 0;
  } else {
    m_regs.at(index) = val;
  }
}
}
