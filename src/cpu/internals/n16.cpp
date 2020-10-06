#include "tmbl/config.h"
#include "tmbl/cpu/internals/n16.h"

namespace tmbl {

n16::n16(const uint16 n) : m_data(n) {}

const uint8 n16::hi() const noexcept { return (m_data & reset_lower) >> 8; }
const uint8 n16::lo() const noexcept { return m_data & reset_upper; }

uint16 n16::value() const noexcept { return m_data; }

}
