#include "tmbl/config.h"
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

r8 &r8::operator++() noexcept {
  m_data++;
  return *this;
}

const r8 r8::operator++(int /*dummy*/) noexcept {
  r8 temp = *this;
  ++*this;
  return temp;
}

r8 &r8::operator--() noexcept {
  m_data--;
  return *this;
}

const r8 r8::operator--(int /*dummy*/) noexcept {
  r8 temp = *this;
  --*this;
  return temp;
}

uint8 r8::value() const noexcept { return m_data; }

r8 operator+(const r8 l, const r8 r) {
  byte val = l.value() + r.value();

  r8 k;
  k = val;
  return k;
}

uint8 operator+(const r8 r, const uint8 u) { return r.value() + u; }
bool operator==(const r8 r, uint8 u) { return r.value() == u; }

bool operator==(const r8 left, const r8 right) { return left.value() == right.value(); }
bool operator!=(const r8 left, const r8 right) { return !(left == right); }

bool operator>(const r8 left, const r8 right) { return left.value() > right.value(); }
bool operator<(const r8 left, const r8 right) { return left.value() < right.value(); }

bool operator<=(const r8 left, const r8 right) { return !(left > right); }
bool operator>=(const r8 left, const r8 right) { return !(left < right); }

}

