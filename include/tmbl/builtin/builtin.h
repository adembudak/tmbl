#ifndef BUILTIN_H
#define BUILTIN_H

#include "tmbl/config.h"

#include <cstddef>
#include <array>

namespace tmbl {

class builtin {
public:
  byte readWRAM(const std::size_t index) const noexcept;
  void writeWRAM(const std::size_t index, const byte val) noexcept;

  byte readHRAM(const std::size_t index) const noexcept;
  void writeHRAM(const std::size_t index, const byte val) noexcept;

  byte readEcho(const std::size_t index) const noexcept;
  void writeEcho(const std::size_t index, const byte val) noexcept;

private:
  std::array<byte, 8_KB> m_wram{};
  std::array<byte, 127_B> m_hram{};
};

}

#endif
