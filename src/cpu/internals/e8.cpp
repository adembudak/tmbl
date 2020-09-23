#include "tmbl/config.h"
#include "tmbl/cpu/internals/e8.h"

namespace tmbl {
e8::e8(const int8 i) : m_data(i) {}
int8 e8::loNibble() const noexcept { return m_data & 0b0000'1111; }
int8 e8::hiNibble() const noexcept { return (m_data & 0b1111'0000) >> 4; }
int8 e8::value() const noexcept { return m_data; }
}
