#include "reg16.h"
#include "reg8.h"

namespace tmbl::cpu {

reg16::reg16(const reg8 r8_) noexcept { m_data = r8_.data(); }
reg16::reg16(const uint16 val_) noexcept { m_data = val_; }

reg16 &reg16::operator=(const reg8 r8_) noexcept {
  m_data = r8_.data();
  return *this;
}

reg16 &reg16::operator=(const uint16 val_) {
  m_data = val_;
  return *this;
}

reg16 &reg16::operator=(const uint8 val_) {
  m_data = val_;
  return *this;
}

reg16 &reg16::operator+=(const std::size_t n_) {
  m_data += n_;
  return *this;
}

void reg16::lo(const reg8 val_) {
  m_data = (m_data & ~lower_byte_mask) | (val_.data() & lower_byte_mask);
}

[[nodiscard]] reg8 reg16::lo() const noexcept {
  return reg8(m_data & lower_byte_mask);
}

void reg16::hi(const reg8 val_) {
  m_data = (m_data & ~upper_byte_mask) | (val_.data() & upper_byte_mask);
}

[[nodiscard]] reg8 reg16::hi() const noexcept {
  return reg8((m_data & upper_byte_mask) >> 8);
}

[[nodiscard]] const uint16 reg16::data() const noexcept { return m_data; }

}
