#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "tmbl/config.h" // for byte type

#include "type/rom.h"
#include "type/mbc1.h"
#include "type/mbc2.h"
/*
#include "type/mbc3.h"
#include "type/mbc5.h"
#include "type/mbc6.h"
#include "type/mbc7.h"
*/

#include <cstddef>
#include <filesystem>
#include <vector>
#include <string>
#include <variant>

namespace tmbl {

class cartridge {
public:
  bool init(const std::filesystem::path p) noexcept;
  bool CGB() const noexcept;

  byte readXRAM(const std::size_t index);
  void writeXRAM(const std::size_t index, const byte val);

  byte readROM(const std::size_t index);
  void writeROM(const std::size_t index, const byte val);

private:
  bool m_cgb_support = false; // support for color gameboy functions?
  std::variant<std::monostate, rom, mbc1, mbc2 /*, mbc3, mbc5, mbc6, mbc7*/> pak;
};
}

#endif
