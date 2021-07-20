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
  builtin(cartridge &cart_, registers &regs_);

  byte readWRAM(const std::size_t index) const noexcept;
  void writeWRAM(const std::size_t index, const byte val) noexcept;

  byte readHRAM(const std::size_t index) const noexcept;
  void writeHRAM(const std::size_t index, const byte val) noexcept;

  byte readEcho(const std::size_t index) const noexcept;
  void writeEcho(const std::size_t index, const byte val) noexcept;

  void adjustWRAMSize() noexcept;

private:
  registers &m_regs;
  byte &SVBK;

  cartridge &m_cart;
  cflag &color_gameboy_support;

  std::vector<byte> m_wram;
  std::array<byte, 127_B> m_hram{};
};

}

#endif
