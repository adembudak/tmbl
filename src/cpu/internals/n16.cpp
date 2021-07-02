#include "tmbl/config.h"
#include "tmbl/cpu/internals/n16.h"

namespace tmbl {

n16::n16(const uint16 nn) : m_data(nn) {}

uint8 n16::hi() noexcept { return (m_data & reset_lower) >> 8; }
uint8 n16::lo() noexcept { return m_data & reset_upper; }

const uint8 n16::hi() const noexcept { return (m_data & reset_lower) >> 8; }
const uint8 n16::lo() const noexcept { return m_data & reset_upper; }

uint16 n16::value() const noexcept { return m_data; }

}
