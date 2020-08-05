#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

namespace tmbl {

// type definitions
using n8 = std::uint_fast8_t;
using n16 = std::uint_fast16_t;
using i8 = std::int_fast16_t;
using byte = std::uint8_t;

using flag = bool;
using cflag = const flag;

constexpr cflag set = true;
constexpr cflag reset = false;

constexpr cflag on = true;
constexpr cflag off = false;

// helper functions
constexpr std::size_t operator""_B(unsigned long long val) { return val; }
constexpr std::size_t operator""_KB(unsigned long long val) { return val * 1024; }
constexpr std::size_t operator""_MB(unsigned long long val) { return val * 1024 * 1024; }

}

#endif
