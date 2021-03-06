#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "tmbl/config.h" // for byte type

#include "type/rom.h"
#include "type/mbc1.h"
#include "type/mbc2.h"
#include "type/mbc5.h"
/*
#include "type/mbc3.h"
#include "type/mbc6.h"
#include "type/mbc7.h"
*/

#include <cstddef>
#include <filesystem>
#include <variant>

namespace tmbl {

enum class console : uint8 { dmg, cgb_compatible, cgb_only };
class cartridge {
public:
  void init(const std::filesystem::path &p) noexcept;

  byte readXRAM(const std::size_t index) const noexcept;
  void writeXRAM(const std::size_t index, const byte val) noexcept;

  byte readROM(const std::size_t index) const noexcept;
  void writeROM(const std::size_t index, const byte val) noexcept;

  cflag &cgbSupport() const noexcept;
  console type() const noexcept;

private:
  console m_type;
  flag m_color_gameboy_support;

  std::variant<std::monostate, rom, mbc1, mbc2, mbc5 /*, mbc3, mbc6, mbc7*/> pak;
};
}

#endif
