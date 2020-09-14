#include "tmbl/cpu/internals/r16.h"

namespace tmbl {

r8 &r16::hi() noexcept { return m_hi; }
r8 &r16::lo() noexcept { return m_lo; }

uint16 r16::value() const noexcept { return m_hi.value() << 8U | m_lo.value(); }

}

