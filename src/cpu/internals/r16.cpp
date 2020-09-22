#include "tmbl/cpu/internals/r16.h"

namespace tmbl {

r8 &r16::hi() noexcept { return m_hi; }
r8 &r16::lo() noexcept { return m_lo; }

const r8 &r16::hi() const noexcept { return m_hi; }
const r8 &r16::lo() const noexcept { return m_lo; }

uint16 r16::value() const noexcept { return m_hi.value() << 8U | m_lo.value(); }

r16 operator+(r16 l, const r16 r) {
  auto val = l.value() + r.value();
  r16 ret;

  ret.lo() = val & 0b0000'0000'1111'1111;
  ret.hi() = (val & 0b1111'1111'0000'0000) >> 8;
  return ret;
}

}

