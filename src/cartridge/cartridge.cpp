#include "tmbl/config.h"
#include "tmbl/memory_map.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/cartridge/type/rom.h"
#include "tmbl/cartridge/type/mbc1.h"
#include "tmbl/cartridge/type/mbc2.h"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <variant>
#include <vector>
#include <numeric> // for std::accumulate

#include <iostream>
#include <cassert>

#if defined PRINT_PAK_INFO
#include <algorithm> // for std::copy_n
#include "tmbl/cartridge/metadata/pakInfo.h"
#endif

namespace tmbl {

bool cartridge::init(const std::filesystem::path p) {
  if (std::filesystem::exists(p)) {

    std::fstream f(p, std::ios::in | std::ios::binary);
    if (!f.good()) {
      std::cerr << "Something wrong with the rom.\n";
      return false;
    }

    auto dumpedGamePak = std::vector<byte>(std::istreambuf_iterator<char>(f), {});

#if PRINT_PAK_INFO
    std::array<byte, 336_B> cartridge_header;
    std::copy_n(begin(dumpedGamePak), 0x014F + 1, begin(cartridge_header));
    std::cerr << metadata::pakInfo(std::move(cartridge_header));
#endif

    // whether the cart support for color gameboy
    const std::size_t CGB_support_code = 0x0143;
    m_cgb_support =
        (dumpedGamePak[CGB_support_code] == 0x0080 || dumpedGamePak[CGB_support_code] == 0x00C0);

    // decide to mbc type
    const std::size_t pak_type = 0x0147;
    const std::size_t pak_rom_size = 0x0148; // equals to dumpedGamePak size, so no need to check
    const std::size_t pak_xram_size = 0x0149;

    // header checksum:  https://gbdev.io/pandocs/#_014d-header-checksum
    const std::size_t checksum_begin = 0x0134;
    const std::size_t checksum_end = 0x014C + 1; // +1 for to use the address as an end iterator
    const std::size_t checksum_result = 0x014D;

    int checksum = std::accumulate(&dumpedGamePak[checksum_begin], &dumpedGamePak[checksum_end], 0,
                                   [this](const byte x, const byte y) { return x - y - 1; });

    assert((dumpedGamePak[checksum_result] == checksum) && "ROM Checksum failed\n");

    auto recognize_xram_size = [](const std::size_t i) {
      // clang-format off
        switch (i) {
          case 0: return 0_KB;
          case 1: return 2_KB;
          case 2: return 8_KB;
          case 3: return 32_KB;
          case 4: return 128_KB;
          case 5: return 64_KB;
          default: break;
          // clang-format on
      }
    };

    switch (dumpedGamePak.at(pak_type)) { // decide pak type
      case 0x00:
        pak = rom(std::move(dumpedGamePak), 0);
        break;

      case 0x01:
        pak = mbc1(std::move(dumpedGamePak));
        break;

      case 0x02:
        [[fallthrough]];
      case 0x03:
        pak = mbc1(std::move(dumpedGamePak), recognize_xram_size(dumpedGamePak.at(pak_xram_size)));
        break;

      case 0x05:
        [[fallthrough]];
      case 0x06:
        pak = mbc2(std::move(dumpedGamePak));
        break;

      case 0x08:
        [[fallthrough]];
      case 0x09:
        pak = rom(std::move(dumpedGamePak), recognize_xram_size(dumpedGamePak.at(pak_xram_size)));
        break;

      default:
        break;
    }
    return true;
  } else {
    std::cerr << p.string() << " not exist.";
    return false;
  }
}

bool cartridge::CGB() const noexcept { return m_cgb_support; }

byte cartridge::readXRAM(const std::size_t index) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    return pRom->read_xram(index);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    return pMbc1->read(index);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    return pMbc2->read(index);
  } else /*if*/ {
    // other mbc types
  }
}

void cartridge::writeXRAM(const std::size_t index, const byte val) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    pRom->write_xram(index, val);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    pMbc1->write(index, val);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    pMbc2->write(index, val);
  } else /*if*/ {
    // other mbc types
  }
}

byte cartridge::readROM(const std::size_t index) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    return pRom->read_rom(index);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    return pMbc1->read(index);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    return pMbc2->read(index);
  } else /*if*/ {
    // other mbc types
  }
}

void cartridge::writeROM(const std::size_t index, const byte val) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    pRom->write_rom(index, val);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    pMbc1->write(index, val);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    pMbc2->write(index, val);
  } else /*if*/ {
    // other mbc types
  }
}

}
