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

};

}

#endif
