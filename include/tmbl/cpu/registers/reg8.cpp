#include "reg8.h"
#include "reg16.h"

namespace tmb::cpu {

explicit reg8::reg8(const uint8 val_) noexcept { m_data = val_; }

void reg8::Z(flag val_ = true) noexcept {
  val_ ? m_data |= 0b1000'0000 : m_data &= 0b0111'1111;
}

void reg8::N(flag val_ = true) noexcept {
  val_ ? m_data |= 0b0100'0000 : m_data &= 0b1011'1111;
}

void reg8::H(flag val_ = true) noexcept {
  val_ ? m_data |= 0b0010'0000 : m_data &= 0b1101'1111;
}

void reg8::C(flag val_ = true) noexcept {
  val_ ? m_data |= 0b0001'0000 : m_data &= 0b1110'1111;
}

const reg8::reg8 &operator=(const uint8 val_) {
  if (val_ < 0 || val_ > 255)
    throw std::out_of_range("8 bit register value must be in range [0, 255)");

  m_data = val_;
  return *this;
}

const reg8::reg8 &operator=(const reg16 r16_) {
  if (auto val = r16_.data(); val < 0 || val > 255)
    throw std::out_of_range("8 bit register value must be in range [0, 255)");

  m_data = r16_.data();
  return *this;
}

[[nodiscard]] const bool reg8::operator==(const uint8 val_) {
  return m_data == val_;
}

[[nodiscard]] const bool reg8::operator==(const reg8 r8_) {
  return m_data == r8_.data();
}

[[nodiscard]] const uint8 reg8::data() const noexcept { return m_data; }

}
