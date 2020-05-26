#include "reg8.h"
#include "reg16.h"

namespace tmbl::cpu {

reg8::reg8(const uint8 val_) noexcept { m_data = val_; }

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

reg8 &reg8::operator=(const uint8 val_) {
  m_data = val_;
  return *this;
}

reg8 &reg8::operator=(const reg16 r16_) {
  m_data = r16_.data();
  return *this;
}

[[nodiscard]] bool reg8::operator==(const uint8 val_) { return m_data == val_; }
[[nodiscard]] bool reg8::operator==(const reg8 r8_) {
  return m_data == r8_.data();
}

[[nodiscard]] const uint8 reg8::data() const noexcept { return m_data; }

}
