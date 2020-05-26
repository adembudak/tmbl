#pragma once
#include <cstdint>
#include <type_traits>

namespace tmbl {

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using byte = char;
using flag = const bool;

template <typename T>
struct is_regular
    : std::integral_constant<bool, std::is_default_constructible_v<T> &&
                                       std::is_copy_constructible_v<T> &&
                                       std::is_move_constructible_v<T> &&
                                       std::is_copy_assignable_v<T> &&
                                       std::is_move_assignable_v<T>> {};

template <typename T>
static constexpr const bool is_regular_v = is_regular<T>::value;

}
