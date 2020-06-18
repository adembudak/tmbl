#include "memory.h"
#include "../cpu/registers/reg16.h"
#include "../cpu/registers/reg8.h"
#include "../config.h"

namespace tmbl::memory {

[[nodiscard]] bool memory::dumpROM(const char *romfile) { return {}; }
[[nodiscard]] byte *memory::data() const noexcept { return m_data.data(); }

[[nodiscard]] byte &memory::operator[](const cpu::reg8 r) { return m_data.at(r.value()); }
[[nodiscard]] byte &memory::operator[](const cpu::reg16 rr) { return m_data.at(rr.value()); }

[[nodiscard]] byte &memory::operator[](const u8 n) { return m_data.at(n); }
[[nodiscard]] byte &memory::operator[](const u16 nn) { return m_data.at(nn); }
}
