#include "tmbl/io/registers.h"

namespace tmbl {
byte &registers::getAt(const std::size_t index) { return regs.at(index - 0xFF00); }

void registers::write(const std::size_t index, const byte val) {
  const std::size_t normalized_index = index - 0xFF00;
  regs.at(normalized_index) = val;
}

}

