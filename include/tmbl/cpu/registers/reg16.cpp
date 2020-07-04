#include "reg16.h"
#include "reg8.h"

namespace tmbl {

reg16 &reg16::operator=(const u16 nn) {
  m_data = nn;
  return *this;
}

reg16 &reg16::operator+=(const u8 n) {
  m_data += n;
  return *this;
}

reg16 &reg16::operator-=(const u8 n) {
  m_data -= n;
  return *this;
}

const reg16 reg16::operator++(int) {
  m_data++;
  return *this;
}

byte reg16::lo() const noexcept { return m_data & reg16::zeroed_upper_byte_mask; }
byte reg16::hi() const noexcept { return (m_data & reg16::zeroed_lower_byte_mask) >> 8U; }

void reg16::lo(const byte b) noexcept { m_data = (m_data & zeroed_lower_byte_mask) | b; }
void reg16::hi(const byte b) noexcept { m_data = (m_data & zeroed_upper_byte_mask) | (b << 8U); }

void reg16::lo(const reg8 r) noexcept { m_data = (m_data & zeroed_lower_byte_mask) | r.value(); }
void reg16::hi(const reg8 r) noexcept {
  m_data = (m_data & zeroed_upper_byte_mask) | (r.value() << 8U);
}

[[nodiscard]] u16 reg16::value() const noexcept { return m_data; }
[[nodiscard]] u16 operator+(const reg16 rr, const int i) { return rr.value() + i; }
[[nodiscard]] u16 operator-(const reg16 rr, const int i) { return rr.value() - i; }

[[nodiscard]] reg16 operator+(const reg16 rr1, const reg16 rr2) {
  reg16 tmp;
  tmp = rr1.value() + rr2.value();
  return tmp;
}

}

