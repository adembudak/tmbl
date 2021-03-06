#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstddef>

namespace tmbl {

// type definitions
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using int8 = std::int8_t;
using byte = std::uint8_t;

using flag = bool;
using cflag = const flag;

// helper variables
inline constexpr cflag set = true;
inline constexpr cflag reset = !set;

inline constexpr cflag on = set;
inline constexpr cflag off = reset;

// mostly used as container size to increase readability
inline constexpr std::size_t operator""_B(unsigned long long val) { return val; }
inline constexpr std::size_t operator""_KB(unsigned long long val) { return val * 1024; }
inline constexpr std::size_t operator""_MB(unsigned long long val) { return val * 1024 * 1024; }

}

#endif
