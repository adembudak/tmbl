#ifndef MBC1_H
#define MBC1_H

#include "../../config.h"
#include <vector>

namespace tmbl {

class mbc1 final {
public:
  mbc1() = default;
  mbc1(std::vector<char> &&rom, std::size_t xram_size = 0);

  void write(const std::size_t index, const byte b);
  byte read(const std::size_t index);

  enum class mode { rom_banking, ram_banking };

private:
  mode banking_mode = mode::rom_banking;

  std::size_t ram_bank_width = 8_KB;
  std::size_t rom_bank_width = 16_KB;

  std::vector<char> m_rom;
  uint8 lower_rom_bank = 1;
  uint8 upper_rom_bank = 0;
  bool rom_banking_available = false;
  uint8 effective_rom_bank = 0;
  uint8 lower_rom_mask = 0b0001'1111;
  uint8 lower_rom_mask_countl_one = 5;
  uint8 upper_rom_mask = 0b0000'0011;

  std::vector<char> m_xram;
  uint8 ram_bank;
  uint8 ram_mask = 0b0000'0011;
  bool ram_available = false;
  bool ram_banking_available = false;
  bool ram_access_enabled = false;
};
}

#endif

