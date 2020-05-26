#ifndef REG16_H
#define REG16_H

#include "../../config.h"

namespace tmbl::cpu {

class reg8;

class reg16 final {
public:
  reg16() = default;
  explicit reg16(const reg8 r8_) noexcept;
  explicit reg16(const uint16 val_) noexcept;

  reg16 &operator=(const reg8 r8_) noexcept;
  reg16 &operator=(const uint16 val_);
  reg16 &operator=(const uint8 val_);
  reg16 &operator+=(const std::size_t n_);

  void lo(const reg8 val_);
  [[nodiscard]] reg8 lo() const noexcept;

  void hi(const reg8 val_);
  [[nodiscard]] reg8 hi() const noexcept;

  [[nodiscard]] const uint16 data() const noexcept;

private:
  const uint16 upper_byte_mask = 0b0000'0000'1111'1111;
  const uint16 lower_byte_mask = 0b1111'1111'0000'0000;

  mutable uint16 m_data = 0b0000'0000'0000'0000;
};

}

#endif

