#pragma once
#include "../../config.h"

namespace tmbl::cpu {

class reg8;

class reg16 {
public:
  reg16() = default;
  explicit reg16(const reg8 r8_) noexcept;
  explicit reg16(const uint16 val_) noexcept;

  const reg16 &operator=(const reg8 r8_) noexcept;
  const reg16 &operator=(const uint16 val_);
  const reg16 &operator=(const uint8 val_);
  reg16 &operator+=(const std::size_t n_);

  void lo(const reg8 val_);
  [[nodiscard]] const reg8 lo() const noexcept;

  void hi(const reg8 val_);
  [[nodiscard]] const reg8 hi() const noexcept;

  [[nodiscard]] const uint16 data() const noexcept;

private:
  uint16 m_data = 0b0000'0000'0000'0000;
};

}
