#include "reg8.h"
#include <stdexcept>

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

bool reg8::test(std::size_t pos) const {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("Reg8: Attempt to reach out of range\n");
  }

  return ((m_data & (1U << pos)) == 0) ? false : true;
}

void reg8::Z(cflag val_) noexcept { val_ ? m_data |= 0b1000'0000 : m_data &= 0b0111'1111; }
void reg8::N(cflag val_) noexcept { val_ ? m_data |= 0b0100'0000 : m_data &= 0b1011'1111; }
void reg8::H(cflag val_) noexcept { val_ ? m_data |= 0b0010'0000 : m_data &= 0b1101'1111; }
void reg8::C(cflag val_) noexcept { val_ ? m_data |= 0b0001'0000 : m_data &= 0b1110'1111; }

cflag reg8::Z() const noexcept { return (((m_data & 0b1000'0000U) >> 7U) == 1) ? set : reset; }
cflag reg8::N() const noexcept { return (((m_data & 0b0100'0000U) >> 6U) == 1) ? set : reset; }
cflag reg8::H() const noexcept { return (((m_data & 0b0010'0000U) >> 5U) == 1) ? set : reset; }
cflag reg8::C() const noexcept { return (((m_data & 0b0001'0000U) >> 4U) == 1) ? set : reset; }

u8 reg8::loNibble() const noexcept { return m_data & zeroed_upper_byte_mask; }
u8 reg8::hiNibble() const noexcept { return (m_data & zeroed_lower_byte_mask) >> 4U; };

u8 reg8::lsb() const noexcept { return m_data & 0b0000'0001U; }
u8 reg8::msb() const noexcept { return (m_data & 0b1000'0000U) >> 7U; }

u8 reg8::min() noexcept { return min_u8_val; }
u8 reg8::max() noexcept { return max_u8_val; }

[[nodiscard]] u8 reg8::value() const noexcept { return m_data; }

[[nodiscard]] reg8 operator+(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.value() + r2.value();

  tmp = result;
  return tmp;
}

[[nodiscard]] reg8 operator-(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.value() - r2.value();

  tmp = result;
  return tmp;
}

[[nodiscard]] reg8 operator&(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.value() & r2.value();
  tmp = result;

  return tmp;
}

[[nodiscard]] reg8 operator|(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.value() | r2.value();
  tmp = result;

  return tmp;
}

[[nodiscard]] reg8 operator^(const reg8 r1, const reg8 r2) {
  reg8 tmp;
  u8 result = r1.value() ^ r2.value();
  tmp = result;

  return tmp;
}

}
