#include "tmbl/cpu/internals/n8.h"

namespace tmbl {

n8::n8(const uint8 n) : m_data(n) {}
uint8 n8::value() const noexcept { return m_data; }

}
