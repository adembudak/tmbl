#ifndef N16_H
#define N16_H

#include "../../config.h"

namespace tmbl {

class n16 final {
public:
  n16(const uint16 n);

  const uint8 hi() const noexcept;
  const uint8 lo() const noexcept;

  uint16 value() const noexcept;

  static constexpr const uint16 reset_upper = 0b0000'0000'1111'1111;
  static constexpr const uint16 reset_lower = 0b1111'1111'0000'0000;

private:
  uint16 m_data;
};

}

#endif
