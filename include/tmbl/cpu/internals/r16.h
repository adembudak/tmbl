#include "../../config.h"
#include "r8.h"

namespace tmbl {

class r16 final {
public:
  // r16(const r16 &other) noexcept; // ADD HL, r16

  r8 &hi() noexcept;
  r8 &lo() noexcept;

  const r8 &hi() const noexcept;
  const r8 &lo() const noexcept;

  uint16 value() const noexcept;

private:
  r8 m_hi, m_lo;
};

r16 operator+(const r16 l, const r16 r);

}
