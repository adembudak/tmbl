#ifndef ROM_H
#define ROM_H

#include "tmbl/config.h"
#include <cstddef>
#include <vector>

namespace tmbl {
class rom {
public:
  rom() = default;
  explicit rom(std::vector<byte> &&rom_, std::size_t xram_size = 0);

  byte read(const std::size_t index) noexcept;
  void write(const std::size_t index, const byte val) noexcept;

  byte read_xram(const std::size_t index) noexcept;
  void write_xram(const std::size_t index, const byte val) noexcept;

private:
  std::vector<byte> m_xram{};
  bool has_xram = false;

  std::vector<byte> m_rom;
};
}

#endif
