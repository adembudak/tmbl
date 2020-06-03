#ifndef CONFIG_H
#define CONFIG_H

#include <boost/safe_numerics/safe_integer.hpp>

#include <cstddef>

namespace tmbl {

using u8 = boost::safe_numerics::safe<std::uint8_t>;
using u16 = boost::safe_numerics::safe<std::uint16_t>;
using byte = boost::safe_numerics::safe<std::byte>;
using flag = boost::safe_numerics::safe<const bool>;

}

#endif
