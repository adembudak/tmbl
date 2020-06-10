#ifndef CONFIG_H
#define CONFIG_H

#include <boost/safe_numerics/safe_integer.hpp>

#include <cstddef>

namespace tmbl {

using u8 = std::uint_fast8_t;
using u16 = std::uint_fast16_t;
using i8 = std::int_fast16_t;

using byte = std::byte;

using flag = bool;
using cflag = const flag;

constexpr cflag set = true;
constexpr cflag reset = false;
}

#endif
