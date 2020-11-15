#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "../config.h" // for byte type

#include "carttypes/rom/rom.h"
#include "carttypes/mbc1/mbc1.h"
/*
#include "mbc2/mbc2.h"
#include "mbc3/mbc3.h"
#include "mbc5/mbc5.h"
#include "mbc6/mbc6.h"
#include "mbc7/mbc7.h"
*/

#include <cstddef>
#include <filesystem>
#include <vector>
#include <string>
#include <variant>

namespace tmbl {

class cartridge {
public:
  // The section between bytes 0x0100-0x014F of the cartridge provides metadata about the game.
  // Game title, cartridge type, manufacturer code, color gameboy support, super gameboy
  // support and some other informations holded by some byte meaning.
  // see: https://archive.org/details/GameBoyProgManVer1.1/page/n126/mode/1up
  // see: https://archive.org/details/GameBoyProgManVer1.1/page/n294/mode/1up

  bool init(const std::filesystem::path p);

  bool CGB() const noexcept;

  byte readROM(const std::size_t index);
  byte readXRAM(const std::size_t index);

  void writeROM(const std::size_t index, const byte val);
  void writeXRAM(const std::size_t index, const byte val);

private:
  bool m_cgb_support = false; // supports cgb functions?
  std::variant<rom, mbc1 /*,mbc2, mbc3, mbc5, mbc6, mbc7*/> pak;
};
}

#endif
