#ifndef REGISTERS_H
#define REGISTERS_H

#include "tmbl/config.h"
#include <array>

namespace tmbl {

// A class to hold registers:
// see: https://archive.org/details/GameBoyProgManVer1.1/page/n16/mode/1up

class registers {
public:
  byte &getAt(const std::size_t index) noexcept;

  byte read(const std::size_t index) const noexcept;
  void write(const std::size_t index, const byte val) noexcept;

private:
  std::array<byte, 160_B> m_regs{};
};

}

#endif
