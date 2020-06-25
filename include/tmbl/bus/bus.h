#ifndef BUS_H
#define BUS_H

#include "../config.h"
#include <array>

namespace tmbl::cpu {
class reg8;
class reg16;
}

namespace tmbl::bus {

class bus final {
public:
  [[nodiscard]] byte read(const cpu::reg8 r);
  [[nodiscard]] byte read(const cpu::reg16 rr);
  [[nodiscard]] byte read(const u8 n);
  [[nodiscard]] byte read(const u16 nn);

  void write(const cpu::reg8 r, const u8 n);
  void write(const cpu::reg16 rr, const cpu::reg8 r);
  void write(const cpu::reg16 rr, const u16 nn);
  void write(const u16 nn, const cpu::reg8 r);
  void write(const u16 nn, const byte b);
  void write(const cpu::reg16 rr, const byte b);
  void write(const cpu::reg16 rr1, const cpu::reg16 rr2);

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

private:
  mutable std::array<byte, 64 * 1024> m_data{};
};

}

#endif

