#include "tmbl/config.h"
#include "tmbl/cpu/internals/n8.h"

namespace tmbl {

n8::n8(const uint8 n) : m_data(n) {}

uint8 n8::loNibble() const noexcept { return m_data & 0b0000'1111; }
uint8 n8::hiNibble() const noexcept { return (m_data & 0b1111'0000) >> 4; }

uint8 n8::value() const noexcept { return m_data; }

bool operator==(const n8 left, const n8 right) { return left.value() == right.value(); }
bool operator!=(const n8 left, const n8 right) { return !(left == right); }

bool operator<(const n8 left, const n8 right) { return left.value() < right.value(); }
bool operator>(const n8 left, const n8 right) { return left.value() > right.value(); }

bool operator<=(const n8 left, const n8 right) { return !(left > right); }
bool operator>=(const n8 left, const n8 right) { return !(left < right); }

}

