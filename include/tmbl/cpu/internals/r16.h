#ifndef R16_H
#define R16_H

#include "tmbl/config.h"
#include "r8.h"
#include "n16.h"

namespace tmbl {

class r16 final {
public:
  // r16(const r16 &other) noexcept; // ADD HL, r16

  r8 &hi() noexcept;
  r8 &lo() noexcept;

  const r8 &hi() const noexcept;
  const r8 &lo() const noexcept;

  r16 &operator=(const n16 &nn);

  const r16 operator++(int dummy) noexcept;
  const r16 operator--(int dummy) noexcept;

  uint16 value() const noexcept;

  static constexpr const uint16 zero = 0;
  static constexpr const uint16 reset_upper = 0b0000'0000'1111'1111;
  static constexpr const uint16 reset_lower = 0b1111'1111'0000'0000;

  static constexpr const uint16 min = 0b0000'0000'0000'0000;
  static constexpr const uint16 max = 0b1111'1111'1111'1111;

private:
  r8 m_hi, m_lo;
};

r16 operator+(const r16 l, const r16 r);
bool operator==(const r16 rr, const uint16 uu);

}

#endif
