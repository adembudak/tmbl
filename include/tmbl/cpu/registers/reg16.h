#ifndef REG16_H
#define REG16_H

#include "../../config.h"

namespace tmbl::cpu {

class reg16 final {
public:
  reg16 &operator=(const u16 nn);
  reg16 &operator+=(const u8 n);
  reg16 &operator-=(const u8 n);

  byte lo() const noexcept;
  byte hi() const noexcept;

  [[nodiscard]] u16 data() const noexcept;

private:
  mutable u16 m_data = 0b0000'0000'0000'0000;

  static constexpr u16 zeroed_upper_byte_mask = 0b0000'0000'1111'1111;
  static constexpr u16 zeroed_lower_byte_mask = 0b1111'1111'0000'0000;
};

[[nodiscard]] u16 operator-(const reg16 rr, const int i);
[[nodiscard]] u16 operator+(const reg16 rr, const int i);

}
#endif

