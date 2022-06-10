#include "tmbl/config.h"
#include "tmbl/memory_map.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/cartridge/type/rom.h"
#include "tmbl/cartridge/type/mbc1.h"
#include "tmbl/cartridge/type/mbc2.h"
#include "tmbl/cartridge/type/mbc5.h"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <variant>
#include <vector>
#include <numeric> // for std::accumulate

#include <iostream>
#include <cassert>

#if PRINT_PAK_INFO
#include <algorithm> // for std::copy_n
#include "tmbl/cartridge/metadata/pakInfo.h"
#endif

namespace tmbl {

// The section between bytes 0x0100-0x014F of the cartridge provides metadata about the game.
// Game title, cartridge type, manufacturer code, color gameboy support, super gameboy
// support and some other informations holded by some byte meaning.
// see: https://archive.org/details/GameBoyProgManVer1.1/page/n126/mode/1up
// see: https://archive.org/details/GameBoyProgManVer1.1/page/n294/mode/1up

void cartridge::init(const std::filesystem::path &p) noexcept {
  std::ifstream fin(p, std::ios::binary);

  auto dumpedGamePak = std::vector<byte>(std::istreambuf_iterator<char>(fin), {});

#if PRINT_PAK_INFO
  std::array<byte, 336_B> cartridge_header;
  std::copy_n(begin(dumpedGamePak), 0x014F + 1, begin(cartridge_header));
  std::cerr << metadata::pakInfo(cartridge_header);
#endif

  // whether the cart support color gameboy functionalities
  const std::size_t CGB_support_code = 0x0143;

  m_type = dumpedGamePak[CGB_support_code] == 0xC0   ? console::cgb_only
           : dumpedGamePak[CGB_support_code] == 0x80 ? console::cgb_compatible
                                                     : console::dmg;

  if (m_type == console::cgb_only || m_type == console::cgb_compatible) {
    m_color_gameboy_support = set;
  }

  // header checksum:  https://gbdev.io/pandocs/#_014d-header-checksum
  const std::size_t checksum_begin = 0x0134;
  const std::size_t checksum_end = 0x014C + 1; // +1 for to use the address as an end iterator
  const std::size_t checksum_result = 0x014D;

  int checksum = std::accumulate(&dumpedGamePak[checksum_begin], &dumpedGamePak[checksum_end], 0,
                                 [](const byte x, const byte y) { return x - y - 1; });

  assert((dumpedGamePak[checksum_result] == checksum) && "ROM Checksum failed\n");

  // decide to mbc type
  const std::size_t pak_type = 0x0147;
  [[maybe_unused]] const std::size_t pak_rom_size =
      0x0148; // equals to dumpedGamePak size, so no need to check
  const std::size_t pak_xram_size = 0x0149;

  auto recognize_xram_size = [&]() noexcept -> std::size_t {
    // clang-format off
       switch (const size_t i = dumpedGamePak[pak_xram_size]; i) {
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
      pak = rom(std::move(dumpedGamePak));
      break;

    case 0x01:
      pak = mbc1(std::move(dumpedGamePak));
      break;

    case 0x02:
      [[fallthrough]];
    case 0x03:
      pak = mbc1(std::move(dumpedGamePak), recognize_xram_size());
      break;

    case 0x05:
      [[fallthrough]];
    case 0x06:
      pak = mbc2(std::move(dumpedGamePak));
      break;

    case 0x08:
      [[fallthrough]];
    case 0x09:
      pak = rom(std::move(dumpedGamePak), recognize_xram_size());
      break;

    case 0x19:
      pak = mbc5(std::move(dumpedGamePak));
      break;
    case 0x1A:
      [[fallthrough]];
    case 0x1B:
      pak = mbc5(std::move(dumpedGamePak), recognize_xram_size());
      break;
    case 0x1C:
      pak = mbc5(std::move(dumpedGamePak), /*xram_size*/ 0, /*has_rumble*/ true);
      break;
    case 0x1D:
      [[fallthrough]];
    case 0x1E:
      pak = mbc5(std::move(dumpedGamePak), recognize_xram_size(), /*has_rumble*/ true);
      break;

    default:
      break;
  }
}

byte cartridge::readXRAM(const std::size_t index) const noexcept {
  if (auto pRom = std::get_if<rom>(&pak)) {
    return pRom->read_xram(index);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    return pMbc1->read(index);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    return pMbc2->read(index);
  } else if (auto pMbc5 = std::get_if<mbc5>(&pak)) {
    return pMbc5->read(index);
  } else /*if*/ {
    // other mbc types
  }
}

void cartridge::writeXRAM(const std::size_t index, const byte val) noexcept {
  if (auto pRom = std::get_if<rom>(&pak)) {
    pRom->write_xram(index, val);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    pMbc1->write(index, val);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    pMbc2->write(index, val);
  } else if (auto pMbc5 = std::get_if<mbc5>(&pak)) {
    pMbc5->write(index, val);
  } else /*if*/ {
    // other mbc types
  }
}

byte cartridge::readROM(const std::size_t index) const noexcept {
  if (auto pRom = std::get_if<rom>(&pak)) {
    return pRom->read(index);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    return pMbc1->read(index);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    return pMbc2->read(index);
  } else if (auto pMbc5 = std::get_if<mbc5>(&pak)) {
    return pMbc5->read(index);
  } else /*if*/ {
    // other mbc types
  }
}

// Write on Read Only Memory? It's due to bank selection
void cartridge::writeROM(const std::size_t index, const byte val) noexcept {
  if (auto pRom = std::get_if<rom>(&pak)) {
    pRom->write(index, val);
  } else if (auto pMbc1 = std::get_if<mbc1>(&pak)) {
    pMbc1->write(index, val);
  } else if (auto pMbc2 = std::get_if<mbc2>(&pak)) {
    pMbc2->write(index, val);
  } else if (auto pMbc5 = std::get_if<mbc5>(&pak)) {
    pMbc5->write(index, val);
  } else /*if*/ {
    // other mbc types
  }
}

cflag &cartridge::cgbSupport() const noexcept { return m_color_gameboy_support; }

console cartridge::type() const noexcept { return m_type; }
}
