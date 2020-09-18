#include "tmbl/cpu/internals/n16.h"

namespace tmbl {

n16::n16(const uint16 n) : m_data(n) {}
uint16 n16::value() const noexcept { return m_data; }

}
