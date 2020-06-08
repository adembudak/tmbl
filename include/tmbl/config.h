#ifndef CONFIG_H
#define CONFIG_H

#include <boost/safe_numerics/safe_integer.hpp>

#include <cstddef>

namespace tmbl {

using u8 = std::uint_fast8_t;
using u16 = std::uint_fast16_t;
using byte = std::byte;
using flag = const bool;

constexpr flag set = true;
constexpr flag reset = false;
}

#endif
