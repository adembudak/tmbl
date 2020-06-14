#ifndef REG16_H
#define REG16_H

#include "../../config.h"

namespace tmbl::cpu {

class reg8;

class reg16 final {
public:
  reg16 &operator=(const u16 nn);
  reg16 &operator+=(const u8 n);
  reg16 &operator-=(const u8 n);
  const reg16 operator++(int);

  byte lo() const noexcept;
  byte hi() const noexcept;

  void lo(const byte b);
  void hi(const byte b);

  static constexpr u16 min() noexcept { return min_u16_val; }
  static constexpr u16 max() noexcept { return max_u16_val; }

  [[nodiscard]] u16 value() const noexcept;

private:
  mutable u16 m_data = 0b0000'0000'0000'0000;

  static constexpr u16 min_u16_val = 0b0000'0000'0000'0000;
  static constexpr u16 max_u16_val = 0b1111'1111'1111'1111;

  static constexpr u16 zeroed_upper_byte_mask = 0b0000'0000'1111'1111;
  static constexpr u16 zeroed_lower_byte_mask = 0b1111'1111'0000'0000;
};

[[nodiscard]] u16 operator+(const reg16 rr, const int i);
[[nodiscard]] u16 operator-(const reg16 rr, const int i);

[[nodiscard]] reg16 operator+(const reg16 rr1, const reg16 rr2);

}
#endif

