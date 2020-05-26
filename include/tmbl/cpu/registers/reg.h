#pragma once
#include <cstdint>

namespace tmbl {

class reg {
public:
  using reg8 = std::uint8_t;
  using reg16 = std::uint16_t;
  using byte = char;

private:
  static constexpr reg16 PC = 0;
  static constexpr reg8 A = 0;
  static constexpr reg8 B = 0;
  static constexpr reg8 C = 0;
  static constexpr reg8 D = 0;
  static constexpr reg8 E = 0;
  static constexpr reg8 F = 0;
};

}
