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

  byte *const rom0_begin = std::data(m_data) + 0x0000U;
  byte *const rom0_end = std::data(m_data) + 0x3FFFU;

  byte *const romN_begin = std::data(m_data) + 0x4000U;
  byte *const romN_end = std::data(m_data) + 0x07FFFU;

  byte *const vram_begin = std::data(m_data) + 0x8000U;
  byte *const vram_end = std::data(m_data) + 0x9FFFU;

  byte *const sram_begin = std::data(m_data) + 0xA000U;
  byte *const sram_end = std::data(m_data) + 0xBFFFU;

  byte *const wram0_begin = std::data(m_data) + 0xC000U;
  byte *const wram0_end = std::data(m_data) + 0xCFFFU;

  byte *const wramN_begin = std::data(m_data) + 0xD000U;
  byte *const wramN_end = std::data(m_data) + 0xDFFFU;

  byte *const wram_echo_begin = std::data(m_data) + 0xE000U;
  byte *const wram_echo_end = std::data(m_data) + 0xFDFFU;

  byte *const oam_begin = std::data(m_data) + 0xFE00U;
  byte *const oam_end = std::data(m_data) + 0xFE9FU;

  byte *const unused_begin = std::data(m_data) + 0xFEA0U;
  byte *const unused_end = std::data(m_data) + 0xFEFFU;

  byte *const io_begin = std::data(m_data) + 0xFF00U;
  byte *const io_end = std::data(m_data) + 0xFF7FU;

  byte *const hram_begin = std::data(m_data) + 0xFF80U;
  byte *const hram_end = std::data(m_data) + 0xFFFEU;

private:
  mutable std::array<byte, 64 * 1024> m_data{};
};

}

#endif

