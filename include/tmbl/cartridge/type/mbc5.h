#ifndef MBC5_H
#define MBC5_H

#include "tmbl/config.h"
#include <vector>

namespace tmbl {

// Based on: https://hacktixme.ga/GBEDG/mbcs/mbc5

class mbc5 final {
public:
  mbc5() = default;
  explicit mbc5(std::vector<byte> &&rom, std::size_t xram_size = 0, const bool has_rumble = false);

  byte read(const std::size_t index) const noexcept;
  void write(const std::size_t index, const byte val) noexcept;

  std::size_t rom_size() const noexcept;
  std::size_t rom_banks() const noexcept;

  std::size_t xram_size() const noexcept;
  std::size_t xram_banks() const noexcept;

  [[nodiscard]] bool has_xram() const noexcept;
  [[nodiscard]] bool has_rumble() const noexcept;

private:
  std::vector<byte> m_xram;
  bool m_has_xram = false;
  uint8 ram_bank = 0;

  flag xram_access_enabled = false;
  bool m_has_rumble = false;

  std::vector<byte> m_rom;
  uint8 rom_bank_low = 0;
  uint8 rom_bank_high = 0;
};

}

#endif
