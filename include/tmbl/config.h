#ifndef CONFIG_H
#define CONFIG_H

#include <boost/safe_numerics/safe_integer.hpp>
#include <cstdint>
#include <type_traits>

namespace tmbl {

using uint8 = boost::safe_numerics::safe<std::uint8_t>;
using uint16 = boost::safe_numerics::safe<std::uint16_t>;
using byte = boost::safe_numerics::safe<char>;
using flag = boost::safe_numerics::safe<const bool>;

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

#endif
