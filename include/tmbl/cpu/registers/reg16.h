#ifndef REG16_H
#define REG16_H

#include "../../config.h"

namespace tmbl::cpu {

class reg8;

class reg16 final {
public:
  reg16 &operator=(const reg8 r8_) noexcept;

  reg16 &operator=(const uint16 val_);
  reg16 &operator=(const uint8 val_);

  reg16 &operator+=(const std::size_t n_);

  void lo(const reg8 val_);
  [[nodiscard]] reg8 lo() const noexcept;

  void hi(const reg8 val_);
  [[nodiscard]] reg8 hi() const noexcept;

  [[nodiscard]] uint16 data() const noexcept;

private:
  mutable uint16 m_data = 0b0000'0000'0000'0000;
};

static_assert(is_regular_v<reg16>);
}

#endif

