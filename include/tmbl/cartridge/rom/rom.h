#ifndef ROM_H
#define ROM_H

#include "../../config.h"
#include <cstddef>
#include <array>
#include <vector>

namespace tmbl {
class rom {
public:
  rom() = default;
  explicit rom(std::vector<byte> &rom_);

  byte read_rom(const std::size_t index);
  byte read_xram(const std::size_t index);
  byte write_xram(const std::size_t index, const byte val);

private:
  std::vector<byte> m_rom;
  std::array<byte, 8_KB> m_xram{};
};

}

#endif

