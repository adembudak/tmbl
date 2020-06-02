#include "memory.h"
#include "../cpu/registers/reg16.h"
#include "../cpu/registers/reg8.h"
#include "../config.h"

namespace tmbl::memory {

[[nodiscard]] bool memory::dumpROM(const char *romfile) { return {}; }
[[nodiscard]] byte *memory::data() const noexcept { return ram.data(); }

[[nodiscard]] byte &memory::operator[](cpu::reg8 r) { return ram.at(r.data()); }
[[nodiscard]] byte &memory::operator[](cpu::reg16 rr) { return ram.at(rr.data()); }

[[nodiscard]] byte &memory::operator[](u8 n) { return ram.at(n); }
[[nodiscard]] byte &memory::operator[](u16 nn) { return ram.at(nn); }

}
