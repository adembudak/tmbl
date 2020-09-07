#ifndef ROM_H
#define ROM_H

#include "../../config.h"
#include <array>
#include <vector>

namespace tmbl {
class rom {
public:
  rom() = default;
  explicit rom(std::vector<byte> &rom_);

  byte read_rom(const std::size_t index);
  byte read_xram(const std::size_t index);

private:
  std::vector<byte> m_rom;
  std::array<byte, 8_KB> m_xram{};
};

}

#endif
