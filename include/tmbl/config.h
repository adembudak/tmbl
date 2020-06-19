#ifndef CONFIG_H
#define CONFIG_H

#include <cstddef>
#include <cstdint>

namespace tmbl {

using u8 = std::uint_fast8_t;
using u16 = std::uint_fast16_t;
using i8 = std::int_fast16_t;
using byte = std::uint8_t;

using flag = bool;
using cflag = const flag;

constexpr cflag set = true;
constexpr cflag reset = false;

}

#endif
