#include "reg8.h"

namespace tmbl::cpu {

reg8 &reg8::operator=(const u8 n) noexcept {
  m_data = n;
  return *this;
}

reg8 &reg8::operator=(byte b) noexcept {
  m_data = std::to_integer<int>(b);
  return *this;
}

reg8 &reg8::operator+=(const u8 n) noexcept {
  m_data += n;
  return *this;
}

reg8 &reg8::operator-=(const u8 n) noexcept {
  m_data -= n;
  return *this;
}

void reg8::Z(flag val_) noexcept { val_ ? m_data |= 0b1000'0000 : m_data &= 0b0111'1111; }
void reg8::N(flag val_) noexcept { val_ ? m_data |= 0b0100'0000 : m_data &= 0b1011'1111; }
void reg8::H(flag val_) noexcept { val_ ? m_data |= 0b0010'0000 : m_data &= 0b1101'1111; }
void reg8::C(flag val_) noexcept { val_ ? m_data |= 0b0001'0000 : m_data &= 0b1110'1111; }

flag reg8::Z() const noexcept { return (((m_data & 0b1000'0000) >> 8) == 1) ? set : reset; }
flag reg8::N() const noexcept { return (((m_data & 0b0100'0000) >> 7) == 1) ? set : reset; }
flag reg8::H() const noexcept { return (((m_data & 0b0010'0000) >> 6) == 1) ? set : reset; }
flag reg8::C() const noexcept { return (((m_data & 0b0001'0000) >> 5) == 1) ? set : reset; }

u8 reg8::loNibble() const noexcept { return m_data & zeroed_upper_byte_mask; }
u8 reg8::hiNibble() const noexcept { return (m_data & zeroed_lower_byte_mask) >> 4; };

u8 reg8::min() noexcept { return min_u8_val; }
u8 reg8::max() noexcept { return max_u8_val; }

[[nodiscard]] u8 reg8::data() const noexcept { return m_data; }

[[nodiscard]] reg8 operator+(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.data() + r2.data();

  tmp = result;
  return tmp;
}

[[nodiscard]] reg8 operator-(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.data() - r2.data();

  tmp = result;
  return tmp;
}

[[nodiscard]] reg8 operator&(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.data() & r2.data();
  tmp = result;

  return tmp;
}

[[nodiscard]] reg8 operator|(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.data() | r2.data();
  tmp = result;

  return tmp;
}

}
