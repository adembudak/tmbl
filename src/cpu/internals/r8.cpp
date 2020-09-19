#include "tmbl/cpu/internals/r8.h"
#include "tmbl/cpu/internals/n8.h"

namespace tmbl {

r8 &r8::operator=(const byte b) {
  m_data = b;
  return *this;
}

r8 &r8::operator=(const n8 n) {
  m_data = n.value();
  return *this;
}
uint8 r8::loNibble() const noexcept { return m_data & 0b0000'1111; }

uint8 r8::hiNibble() const noexcept { return (m_data & 0b1111'0000) >> 4; }

uint8 r8::value() const noexcept { return m_data; }

r8 operator+(const r8 l, const r8 r) { return r8(l.value() + r.value()); }
uint8 operator+(const r8 r, const uint8 u) { return r.value() + u; }
bool operator==(const r8 r, uint8 u) { return r.value() == u; }

}
