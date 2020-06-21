#ifndef MEMORY_H
#define MEMORY_H

#include "../config.h"
#include <array>

namespace tmbl::cpu {
class reg8;
class reg16;
}

namespace tmbl::memory {

class memory final {
public:
  memory();
  [[nodiscard]] bool dumpROM(const char *romfile);
  [[nodiscard]] byte *data() const noexcept;

  [[nodiscard]] byte &operator[](const u16 nn);

  // port mode registers
  byte &IP() noexcept;
  byte &SB() noexcept;
  byte &SC() noexcept;
  byte &DIV() noexcept;
  byte &TIMA() noexcept;
  byte &TMA() noexcept;
  byte &TAC() noexcept;

  // bank control registers -CGB only
  byte &KEY1() noexcept;
  byte &RP() noexcept;

  // Interrupt flags
  byte &IF() noexcept;
  byte &IE() noexcept;

  // display registers
  byte &LCDC() noexcept;
  byte &STAT() noexcept;
  byte &SCY() noexcept;
  byte &SCX() noexcept;
  byte &LY() noexcept;
  byte &LYC() noexcept;
  byte &DMA() noexcept;
  byte &BGP() noexcept;
  byte &OBP0() noexcept;
  byte &OBP1() noexcept;
  byte &WY() noexcept;
  byte &WX() noexcept;

  // display registers -  CGB only
  byte &HDMA1() noexcept;
  byte &HDMA2() noexcept;
  byte &HDMA3() noexcept;
  byte &HDMA4() noexcept;
  byte &HDMA5() noexcept;
  byte &BCPS() noexcept;
  byte &BCPD() noexcept;
  byte &OCPS() noexcept;
  byte &OCPD() noexcept;

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
  const byte nintendo_logo[48]{0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73,
                               0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F,
                               0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD,
                               0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
                               0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};

  mutable std::array<byte, 64 * 1024> m_data{};

  byte *const rom_begin = std::data(m_data) + 0x0000U;
  byte *const rom_end = std::data(m_data) + 0x07FFFU;

  byte *const vram_begin = std::data(m_data) + 0x8000U;
  byte *const vram_end = std::data(m_data) + 0x9FFFU;

  byte *const sram_begin = std::data(m_data) + 0xA000U;
  byte *const sram_end = std::data(m_data) + 0xBFFFU;

  byte *const wram_begin = std::data(m_data) + 0xC000U;
  byte *const wram_end = std::data(m_data) + 0xDFFFU;

  byte *const wram_echo_begin = std::data(m_data) + 0xE000U;
  byte *const wram_echo_end = std::data(m_data) + 0xFDFFU;

  byte *const oam_begin = std::data(m_data) + 0xFE00U;
  byte *const oam_end = std::data(m_data) + 0xFE9FU;

  byte *const io_begin = std::data(m_data) + 0xFF00U;
  byte *const io_end = std::data(m_data) + 0xFF7FU;

  byte *const hram_begin = std::data(m_data) + 0xFF80U;
  byte *const hram_end = std::data(m_data) + 0xFFFEU;

  struct {
    cartridge_type type;
    cartridge_rom rom;
    cartridge_ram ram;
  } cartridge_info;
};

}

#endif
