#include "cartridge.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <numeric>

namespace tmbl::cartridge {

enum cartridge_type {
  ROM_ONLY = 0x00,

  ROM_MBC1 = 0x01,
  ROM_MBC1_RAM = 0x02,
  ROM_MBC1_RAM_BATTERY = 0x03,

  ROM_MBC2 = 0x05,
  ROM_MBC2_RAM_BATTERY = 0x06,

  ROM_RAM = 0x08,
  ROM_RAM_BATTERY = 0x09,

  ROM_MMM01 = 0x0B,
  ROM_MMM01_SRAM = 0x0C,
  ROM_MMM01_SRAM_BATTERY = 0x0D,

  ROM_MBC3_TIMER_BATTERY = 0x0F,
  ROM_MBC3_TIMER_RAM_BATTERY = 0x10,
  ROM_MBC3 = 0x11,
  ROM_MBC3_RAM = 0x12,
  ROM_MBC3_RAM_BATTERY = 0x13,

  ROM_MBC5 = 0x19,
  ROM_MBC5_RAM = 0x1A,
  ROM_MBC5_RAM_BATTERY = 0x1B,

  ROM_MBC5_RUMBLE = 0x1C,
  ROM_MBC5_RUMBLE_SRAM = 0x1D,
  ROM_MBC5_RUMBLE_SRAM_BATTERY = 0x1E,

  ROM_MBC6_RAM_BATTERY = 0x20,
  ROM_MBC7_RAM_BATTERY_ACCELEROMETER = 0x22,

  POCKET_CAMERA = 0xFC,
  BANDAI_TAMA5 = 0xFD,

  Hudson_HuC_3 = 0xFE,
  Hudson_HuC_1 = 0xFF
};

enum cartridge_rom {
  ROM_KB_32 = 0x00,
  ROM_KB_64 = 0x01,
  ROM_KB_128 = 0x02,
  ROM_KB_256 = 0x03,
  ROM_KB_512 = 0x04,
  ROM_KB_1024 = 0x05,
  ROM_KB_2048 = 0x06,
  ROM_KB_4096 = 0x07,
  ROM_KB_8192 = 0x08,

  ROM_KB_1152 = 0x52,
  ROM_KB_1280 = 0x53,
  ROM_KB_1526 = 0x54
};

enum cartridge_ram {
  RAM_None = 0x00,
  RAM_KB_2 = 0x01,
  RAM_KB_8 = 0x02,
  RAM_KB_32 = 0x03,
  RAM_KB_128 = 0x04,
  RAM_KB_64 = 0x05
};

std::vector<byte> dumpROM(const std::filesystem::path &p) {
  std::ifstream stream(p, std::ios::binary);
  return {std::istreambuf_iterator{stream}, {}};
}

cartridge::cartridge(const std::filesystem::path &p) : rom_data(dumpROM(p)) {
  cartType();
  cartRom();
  cartRam();
  romVersion();

  auto checksum = [this]() {
    return std::accumulate(
        rom_data.data() + 0x0134, rom_data.data() + 0x014D, 0,
        [](const byte sum, const byte val) { return sum - val - 1; });
  }();

  assert(checksum == rom_data[0x014C]);
}

const std::string cartridge::title() const noexcept {
  return {std::data(rom_data) + 0x0134, std::data(rom_data) + 0x013F};
}

std::string cartridge::manufacturer() noexcept {
  return {std::data(rom_data) + 0x013F, std::data(rom_data) + 0x0142};
}

bool cartridge::CGB() const noexcept {
  return rom_data[0x0143] == 0x0080 ? true : false;
}

std::string cartridge::newLicenseCode() noexcept {
  switch (rom_data[0x0144] << 8 | rom_data[0x0145]) {
  case 0x00: return "none";
  case 0x13: return "electronic arts";
  case 0x20: return "kss";
  case 0x25: return "san-x";
  case 0x30: return "viacom";
  case 0x33: return "ocean/acclaim";
  case 0x37: return "taito";
  case 0x41: return "ubi soft";
  case 0x46: return "angel";
  case 0x50: return "absolute";
  case 0x53: return "american sammy";
  case 0x56: return "ljn";
  case 0x59: return "milton bradley";
  case 0x64: return "lucasarts";
  case 0x70: return "infogrames";
  case 0x73: return "sculptured";
  case 0x79: return "accolade";
  case 0x86: return "tokuma shoten i*";
  case 0x92: return "video system";
  case 0x96: return "yonezawa/s'pal";
  case 0x01: return "nintendo";
  case 0x18: return "hudsonsoft";
  case 0x22: return "pow";
  case 0x28: return "kemco japan";
  case 0x31: return "nintendo";
  case 0x34: return "konami";
  case 0x38: return "hudson";
  case 0x42: return "atlus";
  case 0x47: return "pullet-proof";
  case 0x51: return "acclaim";
  case 0x54: return "konami";
  case 0x57: return "matchbox";
  case 0x60: return "titus";
  case 0x67: return "ocean";
  case 0x71: return "interplay";
  case 0x75: return "sci";
  case 0x80: return "misawa";
  case 0x87: return "tsukuda ori*";
  case 0x93: return "ocean/acclaim";
  case 0x97: return "kaneko";
  case 0x08: return "capcom";
  case 0x19: return "b-ai";
  case 0x24: return "pcm complete";
  case 0x29: return "seta";
  case 0x32: return "bandia";
  case 0x35: return "hector";
  case 0x39: return "banpresto";
  case 0x44: return "malibu";
  case 0x49: return "irem";
  case 0x52: return "activision";
  case 0x55: return "hi tech entertainment";
  case 0x58: return "mattel";
  case 0x61: return "virgin";
  case 0x69: return "electronic arts";
  case 0x72: return "broderbund";
  case 0x78: return "t*hq";
  case 0x83: return "lozc";
  case 0x91: return "chun soft";
  case 0x95: return "varie";
  case 0x99: return "pack in soft";
  default: return "";
  }
}

bool cartridge::SGB() const noexcept {
  return rom_data[0x0146] == 0x0003 ? true : false;
}

void cartridge::cartType() noexcept {
  switch (rom_data[0x147]) {
  case cartridge_type::ROM_ONLY:
    mbc_type = std::make_unique<mbc0>();
    has_ram = false;
    has_battery = false;
    has_timer = false;

  case cartridge_type::ROM_MBC1:
    // TODO: implement MBC1
    // mbc_type = std::make_unique<mbc1>();
    has_ram = false;
    // has battery means the game state can be saved.
    has_battery = false;
    has_timer = false;
    break;

  case cartridge_type::ROM_MBC1_RAM:
    // mbc_type = std::make_unique<mbc1>();
    has_ram = true;
    has_battery = false;
    has_timer = false;
    break;

  case cartridge_type::ROM_MBC1_RAM_BATTERY:
    // mbc_type = std::make_unique<mbc1>();
    has_ram = true;
    has_battery = true;
    has_timer = false;
    break;

  case cartridge_type::ROM_MBC2:
    // mbc_type = std::make_unique<mbc2>();
    has_ram = false;
    has_battery = false;
    has_timer = false;
    break;

  case cartridge_type::ROM_MBC2_RAM_BATTERY:
    // mbc_type = std::make_unique<mbc2>();
    has_ram = true;
    has_battery = true;
    has_timer = false;
    break;

  case cartridge_type::ROM_MBC3_TIMER_BATTERY:
    // mbc_type = std::make_unique<mbc3>();
    has_ram = false;
    has_battery = true;
    has_timer = true;
    break;

  case cartridge_type::ROM_MBC3_TIMER_RAM_BATTERY:
    // mbc_type = std::make_unique<mbc3>();
    has_ram = true;
    has_battery = true;
    has_timer = true;
    break;

  case cartridge_type::ROM_MBC3:
    // mbc_type = std::make_unique<mbc3>();
    has_ram = false;
    has_battery = false;
    has_timer = false;
    break;

  case cartridge_type::ROM_MBC3_RAM:
    // mbc_type = std::make_unique<mbc3>();
    // mbc_type = mbc3{std::make_shared<cartridge>(*this)};
    has_ram = true;
    has_battery = false;
    has_timer = false;
    break;

  case cartridge_type::ROM_MBC3_RAM_BATTERY:
    // mbc_type = std::make_unique<mbc3>();
    has_ram = true;
    has_battery = false;
    has_timer = true;
    break;

  default: break;
  }
}

void cartridge::cartRom() noexcept {
  switch (rom_data[0x0148]) {
  case cartridge_rom::ROM_KB_32:
    rom_size = 32 * 1024;
    rom_banks = 2;
    break;

  case cartridge_rom::ROM_KB_64:
    rom_size = 64 * 1024;
    rom_banks = 4;
    break;

  case cartridge_rom::ROM_KB_128:
    rom_size = 128 * 1024;
    rom_banks = 8;
    break;

  case cartridge_rom::ROM_KB_256:
    rom_size = 256 * 1024;
    rom_banks = 16;
    break;

  case cartridge_rom::ROM_KB_512:
    rom_size = 512 * 1024;
    rom_banks = 32;
    break;

  case cartridge_rom::ROM_KB_1024:
    rom_size = 1024 * 1024;
    rom_banks = 64;
    break;

  case cartridge_rom::ROM_KB_2048:
    rom_size = 2048 * 1024;
    rom_banks = 128;
    break;

  case cartridge_rom::ROM_KB_4096:
    rom_size = 4096 * 1024;
    rom_banks = 256;
    break;

  case cartridge_rom::ROM_KB_8192:
    rom_size = 8192 * 1024;
    rom_banks = 512;
    break;
  }
}

void cartridge::cartRam() noexcept {
  switch (rom_data[0x0149]) {
  case cartridge_ram::RAM_None:
    ram_size = 0;
    ram_banks = 0;
    break;

  case cartridge_ram::RAM_KB_2:
    ram_size = 2 * 1024;
    ram_banks = 0;
    break;

  case cartridge_ram::RAM_KB_8:
    ram_size = 8 * 1024;
    ram_banks = 1;
    break;

  case cartridge_ram::RAM_KB_32:
    ram_size = 32 * 1024;
    ram_banks = 4;
    break;

  case cartridge_ram::RAM_KB_128:
    ram_size = 128 * 1024;
    ram_banks = 16;
    break;

  case cartridge_ram::RAM_KB_64:
    ram_size = 64 * 1024;
    ram_banks = 8;
    break;
  }
}

void cartridge::destinationCode() noexcept {
  destination_code = rom_data[0x014A] ? "JP" : "";
}

std::string cartridge::oldLicenseCode() noexcept {
  if (rom_data[0x014B] == 0x0033)
    return newLicenseCode();
  else {
    // typing/editing all of these took less than 5 minutes with Vim <3
    // https://raw.githubusercontent.com/gb-archive/salvage/master/txt-files/gbrom.txt
    switch (rom_data[0x014B]) {
    case 0x00: return "none";
    case 0x09: return "hot-b";
    case 0x0C: return "elite systems";
    case 0x19: return "itc entertainment";
    case 0x1F: return "virgin";
    case 0x28: return "kotobuki systems";
    case 0x31: return "nintendo";
    case 0x34: return "konami";
    case 0x39: return "banpresto";
    case 0x41: return "ubi soft";
    case 0x46: return "angel";
    case 0x4A: return "virgin";
    case 0x50: return "absolute";
    case 0x53: return "american sammy";
    case 0x56: return "ljn";
    case 0x5A: return "mindscape";
    case 0x5D: return "tradewest";
    case 0x67: return "ocean";
    case 0x6F: return "electro brain";
    case 0x72: return "broderbund";
    case 0x78: return "t*hq";
    case 0x7C: return "microprose";
    case 0x83: return "lozc";
    case 0x8C: return "vic tokai";
    case 0x91: return "chun soft";
    case 0x95: return "varie";
    case 0x99: return "arc";
    case 0x9C: return "imagineer";
    case 0xA1: return "hori electric";
    case 0xA6: return "kawada";
    case 0xAA: return "broderbund";
    case 0xAF: return "namco";
    case 0xB2: return "bandai";
    case 0xB7: return "snk";
    case 0xBB: return "sunsoft";
    case 0xC0: return "taito";
    case 0xC4: return "*tokuma shoten i";
    case 0xC8: return "koei";
    case 0xCB: return "vap";
    case 0xCE: return "*pony canyon or";
    case 0xD1: return "sofel";
    case 0xD4: return "ask kodansha";
    case 0xD9: return "banpresto";
    case 0xDD: return "ncs";
    case 0xE0: return "jaleco";
    case 0xE3: return "varie";
    case 0xE8: return "asmik";
    case 0xEB: return "atlus";
    case 0xF0: return "a wave";
    case 0x01: return "nintendo";
    case 0x0A: return "jaleco";
    case 0x13: return "electronic arts";
    case 0x1A: return "yanoman";
    case 0x24: return "pcm complete";
    case 0x29: return "seta";
    case 0x32: return "bandai";
    case 0x35: return "hector";
    case 0x3C: return "*entertainment i";
    case 0x42: return "atlus";
    case 0x47: return "spectrum holoby";
    case 0x4D: return "malibu";
    case 0x51: return "acclaim";
    case 0x54: return "gametek";
    case 0x57: return "matchbox";
    case 0x5B: return "romstar";
    case 0x60: return "titus";
    case 0x69: return "electronic arts";
    case 0x70: return "infogrames";
    case 0x73: return "sculptered soft";
    case 0x79: return "accolade";
    case 0x7F: return "kemco";
    case 0x86: return "*tokuma shoten i";
    case 0x8E: return "ape";
    case 0x92: return "video system";
    case 0x96: return "yonezawa/s'pal";
    case 0x9A: return "nihon bussan";
    case 0x9D: return "banpresto";
    case 0xA2: return "bandai";
    case 0xA7: return "takara";
    case 0xAC: return "toei animation";
    case 0xB0: return "acclaim";
    case 0xB4: return "enix";
    case 0xB9: return "pony canyon";
    case 0xBD: return "sony imagesoft";
    case 0xC2: return "kemco";
    case 0xC5: return "data east";
    case 0xC9: return "ufl";
    case 0xCC: return "use";
    case 0xCF: return "angel";
    case 0xD2: return "quest";
    case 0xD6: return "naxat soft";
    case 0xDA: return "tomy";
    case 0xDE: return "human";
    case 0xE1: return "towachiki";
    case 0xE5: return "epoch";
    case 0xE9: return "natsume";
    case 0xEC: return "epic/sony records";
    case 0xeF: return "xtreme entertainment";
    case 0x08: return "capcom";
    case 0x0B: return "coconuts";
    case 0x18: return "hudsonsoft";
    case 0x1D: return "clary";
    case 0x25: return "san-x";
    case 0x30: return "infogrames";
    case 0x33: return "see above";
    case 0x38: return "capcom";
    case 0x3E: return "gremlin";
    case 0x44: return "malibu";
    case 0x49: return "irem";
    case 0x4F: return "u.s. gold";
    case 0x52: return "activision";
    case 0x55: return "park place";
    case 0x59: return "milton bradley";
    case 0x5C: return "naxat soft";
    case 0x61: return "virgin";
    case 0x6E: return "elite systems";
    case 0x71: return "interplay";
    case 0x75: return "the sales curve";
    case 0x7A: return "triffix entertainment";
    case 0x80: return "misawa entertainment";
    case 0x8B: return "bullet-proof software";
    case 0x8F: return "i'max";
    case 0x93: return "tsuburava";
    case 0x97: return "kaneko";
    case 0x9B: return "tecmo";
    case 0x9F: return "nova";
    case 0xA4: return "konami";
    case 0xA9: return "technos japan";
    case 0xAD: return "toho";
    case 0xB1: return "ascii or nexoft";
    case 0xB6: return "hal";
    case 0xBA: return "*culture brain o";
    case 0xBF: return "sammy";
    case 0xC3: return "squaresoft";
    case 0xC6: return "tonkin house";
    case 0xCA: return "ultra";
    case 0xCD: return "meldac";
    case 0xD0: return "taito";
    case 0xD3: return "sigma enterprises";
    case 0xD7: return "copya systems";
    case 0xDB: return "ljn";
    case 0xDF: return "altron";
    case 0xE2: return "uutaka";
    case 0xE7: return "athena";
    case 0xEA: return "king records";
    case 0xEE: return "igs";
    case 0xFF: return "ljn";
    default: return "";
    }
  }
}

void cartridge::romVersion() noexcept { rom_version = rom_data[0x014C]; }

byte cartridge::headerChecksum() const noexcept {
  return std::accumulate(
      rom_data.data() + 0x0134, rom_data.data() + 0x014D, 0,
      [](const byte sum, const byte val) { return sum - val - 1; });
}

} // namespace tmbl::cartridge
