#include "reg8.h"

namespace tmbl::cpu {

reg8 &reg8::operator=(const u8 n) noexcept {
  m_data = n;
  return *this;
}

void reg8::Z(flag val_) noexcept {
  val_ ? m_data |= 0b1000'0000 : m_data &= 0b0111'1111;
}

void reg8::N(flag val_) noexcept {
  val_ ? m_data |= 0b0100'0000 : m_data &= 0b1011'1111;
}

void reg8::H(flag val_) noexcept {
  val_ ? m_data |= 0b0010'0000 : m_data &= 0b1101'1111;
}

void reg8::C(flag val_) noexcept {
  val_ ? m_data |= 0b0001'0000 : m_data &= 0b1110'1111;
}

[[nodiscard]] u8 reg8::data() const noexcept { return m_data; }

}
