#ifndef BUILTIN_H
#define BUILTIN_H

#include "../config.h"
#include "../memory_map.h"
#include <array>

namespace tmbl {

class builtin {
public:
  byte readWRAM(const std::size_t index);
  void writeWRAM(const std::size_t index, const byte val);

  byte readHRAM(const std::size_t index);
  void writeHRAM(const std::size_t index, const byte val);

private:
  std::array<byte, 8_KB> m_wram{};
  std::array<byte, 160_B> m_hram{};
  std::array<byte, 7_KB + 512_B> m_echo{};
};

}

#endif
