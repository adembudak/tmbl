#ifndef BUILTIN_H
#define BUILTIN_H

#include "tmbl/config.h"

#include <cstddef>
#include <array>

namespace tmbl {

class builtin {
public:
  byte readWRAM(const std::size_t index);
  void writeWRAM(const std::size_t index, const byte val);

  byte readHRAM(const std::size_t index);
  void writeHRAM(const std::size_t index, const byte val);

  byte readEcho(const std::size_t index);
  void writeEcho(const std::size_t index, const byte val);

private:
  std::array<byte, 8_KB> m_wram{};
  std::array<byte, 127_B> m_hram{};
};

}

#endif
