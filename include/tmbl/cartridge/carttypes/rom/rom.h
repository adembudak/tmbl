#ifndef ROM_H
#define ROM_H

#include "../../../config.h"
#include <cstddef>
#include <array>
#include <vector>

namespace tmbl {
class rom {
public:
  rom() = default;
  explicit rom(std::vector<byte> &&rom_, int xram_size);

  byte read_rom(const std::size_t index);
  void write_rom(const std::size_t index, const byte val);

  byte read_xram(const std::size_t index);
  void write_xram(const std::size_t index, const byte val);

private:
  std::vector<byte> m_rom;
  std::vector<byte> m_xram{};
};
}

#endif
