#include "tmbl/config.h"
#include "tmbl/cpu/internals/flags.h"

namespace tmbl {

void flags::z(flag f) { m_z = f; }
void flags::n(flag f) { m_n = f; }
void flags::h(flag f) { m_h = f; }
void flags::c(flag f) { m_c = f; }

flag flags::z() const noexcept { return m_z; }
flag flags::n() const noexcept { return m_n; }
flag flags::h() const noexcept { return m_h; }
flag flags::c() const noexcept { return m_c; }

flags &flags::operator=(const byte b) {
  m_z = (b & 0b1000'0000) >> 7;
  m_n = (b & 0b0100'0000) >> 6;
  m_h = (b & 0b0010'0000) >> 5;
  m_c = (b & 0b0001'0000) >> 4;

  return *this;
}

uint8 flags::value() const noexcept { return (m_z << 7) | (m_n << 6) | (m_h << 5) | (m_c << 4); }

}
