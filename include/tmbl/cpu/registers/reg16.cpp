#include "reg16.h"

namespace tmbl::cpu {

reg16 &reg16::operator=(const u16 nn) {
  m_data = nn;
  return *this;
}

reg16 &reg16::operator+=(const u8 n) {
  m_data += n;
  return *this;
}

reg16 &reg16::operator-=(const u8 n) {
  m_data += n;
  return *this;
}

byte reg16::lo() const noexcept { return byte(m_data & reg16::zeroed_upper_byte_mask); }
byte reg16::hi() const noexcept {
  return byte((m_data & reg16::zeroed_lower_byte_mask) >> 8);
}

void reg16::lo(const byte b) {
  m_data = (m_data & zeroed_lower_byte_mask) | std::to_integer<int>(b);
}

void reg16::hi(const byte b) {
  m_data = ((m_data & zeroed_upper_byte_mask) | std::to_integer<int>(b)) << 8;
}

[[nodiscard]] u16 reg16::data() const noexcept { return m_data; }
[[nodiscard]] u16 operator-(const reg16 rr, const int i) { return rr.data() - i; }
[[nodiscard]] u16 operator+(const reg16 rr, const int i) { return rr.data() + i; }

}

