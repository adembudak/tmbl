#include "cartridge.h"

#include <filesystem>
#include <fstream>
#include <numeric>

namespace tmbl::cartridge {

void cartridge::load(const std::filesystem::path &p) {
  rom_data.reserve(8 * 1024 * 1024);

  std::ifstream stream(p, std::ios::binary);
  std::copy(std::istreambuf_iterator<char>(stream), {}, rom_data.begin());

  rom_data.shrink_to_fit();
}

void cartridge::title() noexcept {
  for (auto start = 0x104; start < 0x134; ++start) {
    game_title.push_back(rom_data[start]);
  }
}

void cartridge::manufacturer() noexcept {
  for (auto start = 0x134; start < 0x143; ++start) {
    manufacturer_code.push_back(rom_data[start]);
  }
}

bool cartridge::CGB() const noexcept {
  return rom_data[0x0143] == 0x0080 ? true : false;
}

void cartridge::newLicenseCode() noexcept {
  new_license_code.push_back(rom_data[0x0144]);
  new_license_code.push_back(rom_data[0x0145]);
}

bool cartridge::SGB() const noexcept {
  return rom_data[0x0146] == 0x0003 ? true : false;
}

void cartridge::cartType() noexcept {
  switch (rom_data[0x147]) {
  case 0x00: cartridge_info.type = cartridge_type::ROM_ONLY; break;

  case 0x01: [[fallthrough]];
  case 0x02: [[fallthrough]];
  case 0x03: cartridge_info.type = cartridge_type::ROM_MBC1;

  case 0x05: [[fallthrough]];
  case 0x06: cartridge_info.type = cartridge_type::ROM_MBC2; break;

  case 0x0B: [[fallthrough]];
  case 0x0C: [[fallthrough]];
  case 0x0D: cartridge_info.type = cartridge_type::ROM_MMM01; break;

  case 0x0F: [[fallthrough]];
  case 0x10: [[fallthrough]];
  case 0x11: [[fallthrough]];
  case 0x12: [[fallthrough]];
  case 0x13: cartridge_info.type = cartridge_type::ROM_MBC3; break;

  case 0x19: [[fallthrough]];
  case 0x1A: [[fallthrough]];
  case 0x1B: [[fallthrough]];
  case 0x1C: [[fallthrough]];
  case 0x1D: [[fallthrough]];
  case 0x1E: cartridge_info.type = cartridge_type::ROM_MBC5; break;
  }
}

void cartridge::cartRom() noexcept {
  switch (rom_data[0x0148]) {
  case 0x00: cartridge_info.rom = cartridge_rom::KB_32; break;
  case 0x01: cartridge_info.rom = cartridge_rom::KB_64; break;
  case 0x02: cartridge_info.rom = cartridge_rom::KB_128; break;
  case 0x03: cartridge_info.rom = cartridge_rom::KB_256; break;
  case 0x04: cartridge_info.rom = cartridge_rom::KB_512; break;
  case 0x05: cartridge_info.rom = cartridge_rom::KB_1024; break;
  case 0x06: cartridge_info.rom = cartridge_rom::KB_2048; break;
  case 0x52: cartridge_info.rom = cartridge_rom::KB_1152; break;
  case 0x53: cartridge_info.rom = cartridge_rom::KB_1280; break;
  case 0x54: cartridge_info.rom = cartridge_rom::KB_1526; break;
  }
}

void cartridge::cartRam() noexcept {
  switch (rom_data[0x0149]) {
  case 0x00: cartridge_info.ram = cartridge_ram::None; break;
  case 0x01: cartridge_info.ram = cartridge_ram::KB_2; break;
  case 0x02: cartridge_info.ram = cartridge_ram::KB_8; break;
  case 0x03: cartridge_info.ram = cartridge_ram::KB_32; break;
  case 0x04: cartridge_info.ram = cartridge_ram::KB_128; break;
  }
}

void cartridge::destinationCode() noexcept {
  destination_code = rom_data[0x014A] ? "JP" : "";
}

void cartridge::oldLicenseCode() noexcept {
  if (rom_data[0x014B] == 0x0033)
    old_license_code = new_license_code;
  else {
    old_license_code.push_back(rom_data[0x014B]);
  }
}

void cartridge::romVersion() noexcept { rom_version = rom_data[0x014C]; }

byte cartridge::headerChecksum() const noexcept {
  return std::accumulate(
      rom_data.data() + 0x0134, rom_data.data() + 0x014D, 0,
      [](const byte sum, const byte val) { return sum - val - 1; });
}

} // namespace tmbl::cartridge
