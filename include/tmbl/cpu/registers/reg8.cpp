#include "reg8.h"
#include "reg16.h"

namespace tmbl::cpu {

reg8 &reg8::operator=(const uint8 val_) noexcept {
  m_data = val_;
  return *this;
}

reg8 &reg8::operator=(const reg16 r_) noexcept {
  m_data = static_cast<uint8>(r_.data() & zeroed_upper_byte_mask);
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

[[nodiscard]] uint8 reg8::data() const noexcept { return m_data; }

bool operator==(const reg8 r_, const uint8 val_) { return r_.data() == val_; }
bool operator==(const reg8 r1_, const reg8 r2_) {
  return r1_.data() == r2_.data();
}

bool operator==(const reg16 r1_, const reg8 r2_) {
  return r1_.data() == r2_.data();
}

bool operator==(const reg8 r1_, const reg16 r2_) {
  return r1_.data() == r2_.data();
}

}
