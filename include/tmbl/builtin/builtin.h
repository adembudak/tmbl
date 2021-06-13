#ifndef BUILTIN_H
#define BUILTIN_H

#include "tmbl/config.h"

#include <cstddef>
#include <vector>
#include <array>

namespace tmbl {
class registers;
class cartridge;

class builtin {
public:
  builtin(registers &regs, cartridge &cart);

  byte readWRAM(const std::size_t index) const noexcept;
  void writeWRAM(const std::size_t index, const byte val) noexcept;

  byte readHRAM(const std::size_t index) const noexcept;
  void writeHRAM(const std::size_t index, const byte val) noexcept;

  byte readEcho(const std::size_t index) const noexcept;
  void writeEcho(const std::size_t index, const byte val) noexcept;

private:
  byte &SVBK;
  registers &m_regs;
  cartridge &m_cart;
  std::vector<byte> m_wram;
  std::array<byte, 127_B> m_hram{};
};

}

#endif
