#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "../config.h" // for byte type

#include "rom/rom.h"
#include "mbc1/mbc1.h"
/*
#include "mbc2/mbc2.h"
#include "mbc3/mbc3.h"
#include "mbc5/mbc5.h"
#include "mbc6/mbc6.h"
#include "mbc7/mbc7.h"
*/

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

  explicit cartridge(const std::filesystem::path &p);

  bool CGB() const noexcept { return m_cgb_support; }
  std::string title() const noexcept { return m_title; }

  std::size_t banks() const noexcept {
    if (CGB()) {
      if (auto pakType = std::get_if<mbc1>(&pak)) {
        return pakType->bankNumber();
      }
    }
  }

  byte read(std::size_t index) {
    if (auto pakType = std::get_if<rom>(&pak)) {
      return pakType->read_rom(index);
    } else if (auto pakType = std::get_if<mbc1>(&pak)) {
      //
    }
  }

private:
  bool m_cgb_support = false; // supports cgb functions?
  std::string m_title;        // game title

  std::variant<rom, mbc1 /*,mbc2, mbc3, mbc5, mbc6, mbc7*/> pak;
  std::vector<byte> dumpedGamePak;
};

}

#endif
