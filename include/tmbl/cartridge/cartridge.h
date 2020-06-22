#ifndef CARTRIDGE_h
#define CARTRIDGE_h

#include "../config.h"
#include <filesystem>
#include <vector>

namespace tmbl::cartridge {

class cartridge final {
public:
   void load(const std::filesystem::path &p);

  bool CGB() const noexcept;
  bool SGB() const noexcept;

  void cartType() noexcept;
  void cartRom() noexcept;
  void cartRam() noexcept;

  enum class cartridge_type {
    ROM_ONLY = 0x00,

    ROM_MBC1 = 0x01,
    ROM_MBC1_RAM = 0x02,
    ROM_MBC1_RAM_BATTERY = 0x03,

    ROM_MBC2 = 0x05,
    ROM_MBC2_BATTERY = 0x06,

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

    POCKET_CAMERA = 0x1F,
    BANDAI_TAMA5 = 0xFD,

    Hudson_HuC_3 = 0xFE,
    Hudson_HuC_1 = 0xFF
  };

  enum class cartridge_rom {
    KB_32 = 0x00,
    KB_64 = 0x01,
    KB_128 = 0x02,
    KB_256 = 0x03,
    KB_512 = 0x04,
    KB_1024 = 0x05,
    KB_2048 = 0x06,
    KB_1152 = 0x52,
    KB_1280 = 0x53,
    KB_1526 = 0x54
  };

  enum class cartridge_ram {
    None = 0x00,
    KB_2 = 0x01,
    KB_8 = 0x02,
    KB_32 = 0x03,
    KB_128 = 0x04,
  };

private:
  struct {
    cartridge_type type;
    cartridge_rom rom;
    cartridge_ram ram;
  } cartridge_info;

  std::vector<byte> rom_data;
};

}

#endif
