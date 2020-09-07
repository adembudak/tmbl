#ifndef MBC1_H
#define MBC1_H

#include "../../config.h"
#include <vector>

namespace tmbl {
class mbc1 {
public:
  mbc1(std::vector<byte> &rom, const std::size_t xram_size = 0);
  std::size_t bankNumber() const noexcept;

  byte read_rom(const std::size_t index);
  byte read_xram(const std::size_t index);

private:
  std::vector<byte> m_xram{};
  std::size_t m_xram_size{};
  std::size_t m_xram_bank{};

  std::vector<byte> m_rom{};
  std::size_t m_rom_bank{};
};
}

#endif
