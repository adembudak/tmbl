#include "bus.h"
#include "../cpu/registers/reg8.h"
#include "../cpu/registers/reg16.h"

namespace tmbl::bus {

[[nodiscard]] byte bus::read(const cpu::reg8 r) { return m_data[r.value()]; }
[[nodiscard]] byte bus::read(const cpu::reg16 rr) { return m_data[rr.value()]; }
[[nodiscard]] byte bus::read(const u8 n) { return m_data[n]; }
[[nodiscard]] byte bus::read(const u16 nn) { return m_data[nn]; }

void bus::write(const cpu::reg8 r, const u8 n) { m_data[r.value()] = n; }
void bus::write(const cpu::reg16 rr, const cpu::reg8 r) { m_data[rr.value()] = r.value(); }
void bus::write(const cpu::reg16 rr, const u16 nn) { m_data[rr.value()] = nn; }
void bus::write(const u16 nn, const cpu::reg8 r) { m_data[nn] = r.value(); }
void bus::write(const u16 nn, const byte b) { m_data[nn] = b; }
void bus::write(const cpu::reg16 rr, const byte b) { m_data[rr.value()] = b; }
void bus::write(const cpu::reg16 rr1, const cpu::reg16 rr2) {
  m_data[rr1.value()] = m_data[rr2.value()];
}

}
