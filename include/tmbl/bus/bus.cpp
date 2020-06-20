#include "bus.h"
#include "../cpu/registers/reg8.h"
#include "../cpu/registers/reg16.h"

namespace tmbl::bus {

[[nodiscard]] byte bus::read(const cpu::reg8 r) { return m[r.value()]; }
[[nodiscard]] byte bus::read(const cpu::reg16 rr) { return m[rr.value()]; }
[[nodiscard]] byte bus::read(const u8 n) { return m[n]; }
[[nodiscard]] byte bus::read(const u16 nn) { return m[nn]; }

void bus::write(const cpu::reg8 r, const u8 n) { m[r.value()] = n; }
void bus::write(const cpu::reg16 rr, const cpu::reg8 r) { m[rr.value()] = r.value(); }
void bus::write(const cpu::reg16 rr, const u16 nn) { m[rr.value()] = nn; }
void bus::write(const u16 nn, const cpu::reg8 r) { m[nn] = r.value(); }
void bus::write(const u16 nn, const byte b) { m[nn] = b; }
void bus::write(const cpu::reg16 rr, const byte b) { m[rr.value()] = b; }
void bus::write(const cpu::reg16 rr1, const cpu::reg16 rr2) { m[rr1.value()] = m[rr2.value()]; }

}
