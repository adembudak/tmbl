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

byte &bus::IP() noexcept { return m_data[0xFF00]; }
byte &bus::SB() noexcept { return m_data[0xFF01]; }
byte &bus::SC() noexcept { return m_data[0xFF02]; }
byte &bus::DIV() noexcept { return m_data[0xFF04]; }
byte &bus::TIMA() noexcept { return m_data[0xFF05]; }
byte &bus::TMA() noexcept { return m_data[0xFF06]; }
byte &bus::TAC() noexcept { return m_data[0xFF07]; }

byte &bus::KEY1() noexcept { return m_data[0xFF4D]; }
byte &bus::RP() noexcept { return m_data[0xFF56]; }

byte &bus::IF() noexcept { return m_data[0xFF0F]; }
byte &bus::IE() noexcept { return m_data[0xFFFF]; }

byte &bus::LCDC() noexcept { return m_data[0xFF40]; }
byte &bus::STAT() noexcept { return m_data[0xFF41]; }
byte &bus::SCY() noexcept { return m_data[0xFF42]; }
byte &bus::SCX() noexcept { return m_data[0xFF43]; }
byte &bus::LY() noexcept { return m_data[0xFF44]; }
byte &bus::LYC() noexcept { return m_data[0xFF45]; }
byte &bus::DMA() noexcept { return m_data[0xFF46]; }
byte &bus::BGP() noexcept { return m_data[0xFF47]; }
byte &bus::OBP0() noexcept { return m_data[0xFF48]; }
byte &bus::OBP1() noexcept { return m_data[0xFF49]; }
byte &bus::WY() noexcept { return m_data[0xFF4A]; }
byte &bus::WX() noexcept { return m_data[0xFF4B]; }

byte &bus::HDMA1() noexcept { return m_data[0xFF51]; }
byte &bus::HDMA2() noexcept { return m_data[0xFF52]; }
byte &bus::HDMA3() noexcept { return m_data[0xFF53]; }
byte &bus::HDMA4() noexcept { return m_data[0xFF54]; }
byte &bus::HDMA5() noexcept { return m_data[0xFF55]; }
byte &bus::BCPS() noexcept { return m_data[0xFF68]; }
byte &bus::BCPD() noexcept { return m_data[0xFF69]; }
byte &bus::OCPS() noexcept { return m_data[0xFF6A]; }
byte &bus::OCPD() noexcept { return m_data[0xFF6B]; }

}
