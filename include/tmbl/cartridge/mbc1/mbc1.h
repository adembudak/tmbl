#ifndef MBC1_H
#define MBC1_H

#include "../../config.h"
#include <vector>

namespace tmbl {

class mbc1 final {
public:
  mbc1(std::vector<byte> &rom, std::size_t xram_size = 0);
  void write(const std::size_t index, const byte b);

  enum class mode { rom_banking, ram_banking };
private:
  std::vector<byte> m_xram;
  std::vector<byte> m_rom;

  mode banking_mode = mode::rom_banking;

  std::size_t ram_bank_width = 8_KB;
  std::size_t rom_bank_width = 16_KB;

  uint8 lower_rom_bank = 1; // holds first 5 bits of rom bank number
  uint8 upper_rom_bank = 0; // holds bits "upper" than first 5 bits
  uint8 effective_rom_bank = 0;
  uint8 lower_rom_mask = 0b0001'1111;
  uint8 upper_rom_mask = 0b0000'0011;

  uint8 ram_bank;
  uint8 ram_mask = 0b0000'0011;
  bool ram_access_enabled = false;
};
}

#endif
