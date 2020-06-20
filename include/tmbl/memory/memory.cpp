#include "memory.h"
#include "../cpu/registers/reg16.h"
#include "../cpu/registers/reg8.h"
#include "../config.h"
#include <algorithm>

namespace tmbl::memory {
memory::memory() {
  std::copy(std::begin(nintendo_logo), std::end(nintendo_logo), std::data(m_data) + 0x104);
}

[[nodiscard]] bool memory::dumpROM(const char *romfile) { return {}; }
[[nodiscard]] byte *memory::data() const noexcept { return m_data.data(); }

[[nodiscard]] byte &memory::operator[](const u16 nn) { return m_data.at(nn); }
}
