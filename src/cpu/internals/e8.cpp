#include "tmbl/cpu/internals/e8.h"

namespace tmbl {
e8::e8(const int8 i) : m_data(i) {}
int8 e8::value() const noexcept { return m_data; }
}
