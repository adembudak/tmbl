#include "tmbl/config.h"
#include "tmbl/cartridge/cartridge.h"

#include <vector>
#include <fstream>
#include <iterator>
#include <filesystem>

namespace tmbl {

inline std::vector<byte> dumpROM(const std::filesystem::path &p) {
  std::fstream f(p, std::ios::binary);
  return {std::istreambuf_iterator(f), {}};
}

cartridge::cartridge(const std::filesystem::path &p) : dumpedGamePak(dumpROM(p)) {

  const std::size_t title = 0x0134;
  const std::size_t title_end = 0x013E + 1; // +1 for the use of iterator overload of std::string.
  m_title = std::string(&dumpedGamePak[title], &dumpedGamePak[title_end]);

  const std::size_t CGB_support_code = 0x0143;
  if (dumpedGamePak[CGB_support_code] == 0x0080 || dumpedGamePak[CGB_support_code] == 0x00C0) {
    m_cgb_support = true;
  } else {
    m_cgb_support = false;
  }

  const std::size_t pak_type = 0x0147;
  const std::size_t pak_rom_size = 0x0148;
  const std::size_t pak_xram_size = 0x0149;

  switch (dumpedGamePak[pak_type]) {
    case 0x00:
      pak = rom{dumpedGamePak};
      break;

    case 0x01:
      pak = mbc1{dumpedGamePak};
      break;

    case 0x02:
      [[fallthrough]];
    case 0x03:
      switch (dumpedGamePak[pak_xram_size]) {
        case 0x00:
          pak = mbc1{dumpedGamePak};
          break;

        case 0x01:
          pak = mbc1{dumpedGamePak, /*xram_size=*/2_KB};
          break;

        case 0x02:
          pak = mbc1{dumpedGamePak, /*xram_size=*/8_KB};
          break;

        case 0x03:
          pak = mbc1{dumpedGamePak, /*xram_size=*/32_KB};
          break;

        case 0x04:
          pak = mbc1{dumpedGamePak, /*xram_size=*/128_KB};
          break;

        case 0x05:
          pak = mbc1{dumpedGamePak, /*xram_size=*/64_KB};
          break;

        default:
          break;
      }
  }
}
}
