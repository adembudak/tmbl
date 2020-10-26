#include "tmbl/config.h"
#include "tmbl/memory_map.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/cartridge/mbc1/mbc1.h"
#include "tmbl/cartridge/rom/rom.h"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <variant>
#include <vector>
#include <numeric> // for std::accumulate

#include <iostream>
#include <cassert>

namespace tmbl {

void cartridge::init(const std::filesystem::path p) {

  std::fstream f(p, std::ios::in | std::ios::binary);
  if (!f.good()) {
    std::cerr << "Something wrong with the rom.\n";
  }

  dumpedGamePak = std::vector<char>(std::istreambuf_iterator<char>(f), {});

  // decide game title
  const std::size_t title = 0x0134;
  const std::size_t title_end = 0x013E + 1; // +1 to use iterator overload of std::string ctor
  m_title = std::string(&dumpedGamePak[title], &dumpedGamePak[title_end]);

  // whether the cart support for color gameboy
  const std::size_t CGB_support_code = 0x0143;
  if (dumpedGamePak[CGB_support_code] == 0x0080 || dumpedGamePak[CGB_support_code] == 0x00C0) {
    m_cgb_support = true;
  } else {
    m_cgb_support = false;
  }

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

  switch (dumpedGamePak[pak_type]) { // decide pak type
    case 0x00:
      pak = rom(std::move(dumpedGamePak), 0);
      break;

    case 0x01:
      pak = mbc1(std::move(dumpedGamePak));
      break;

    case 0x02:
      [[fallthrough]];
    case 0x03:
      pak = mbc1(std::move(dumpedGamePak), recognize_xram_size(dumpedGamePak[pak_xram_size]));
      break;

    case 0x08:
      [[fallthrough]];
    case 0x09:
      pak = rom(std::move(dumpedGamePak), recognize_xram_size(dumpedGamePak[pak_xram_size]));
      break;

    default:
      break;
  }
}

bool cartridge::CGB() const noexcept { return m_cgb_support; }

std::string cartridge::title() const noexcept { return m_title; }

byte cartridge::readROM(const std::size_t index) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    return pRom->read_rom(index);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    return pMbc1->read(index);
  } else /*if*/ {
    // other mbc types
  }
}

byte cartridge::readXRAM(const std::size_t index) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    return pRom->read_xram(index);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    return pMbc1->read(index);
  } else /*if*/ {
    // other mbc types
  }
}
void cartridge::writeROM(const std::size_t index, const byte val) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    std::cerr << "Only mbc type carts can take ROM area address to write (to set bank registers.\n";
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    pMbc1->write(index, val);
  } else /*if*/ {
    // other mbc types
  }
}

void cartridge::writeXRAM(const std::size_t index, const byte val) {
  if (auto pRom = std::get_if<rom>(&pak)) {
    pRom->write_xram(index, val);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    pMbc1->write(index, val);
  } else /*if*/ {
    // other mbc types
  }
}

}
