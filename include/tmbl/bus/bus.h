#ifndef BUS_H
#define BUS_H

#include "../config.h"
#include "../cartridge/cartridge.h"
#include <array>
#include <memory>

namespace tmbl::cpu {
class reg8;
class reg16;
}

namespace tmbl::bus {

class bus final {
public:
  bus() {

    std::copy(std::cbegin(bootrom), std::cend(bootrom), m_data.data() + 104);

    m_data[0xFF05] = 0x00; // TIMA
    m_data[0xFF06] = 0x00; // TMA
    m_data[0xFF07] = 0x00; // TAC
    m_data[0xFF10] = 0x80; // NR10
    m_data[0xFF11] = 0xBF; // NR11
    m_data[0xFF12] = 0xF3; // NR12
    m_data[0xFF14] = 0xBF; // NR14
    m_data[0xFF16] = 0x3F; // NR21
    m_data[0xFF17] = 0x00; // NR22
    m_data[0xFF19] = 0xBF; // NR24
    m_data[0xFF1A] = 0x7F; // NR30
    m_data[0xFF1B] = 0xFF; // NR31
    m_data[0xFF1C] = 0x9F; // NR32
    m_data[0xFF1E] = 0xBF; // NR33
    m_data[0xFF20] = 0xFF; // NR41
    m_data[0xFF21] = 0x00; // NR42
    m_data[0xFF22] = 0x00; // NR43
    m_data[0xFF23] = 0xBF; // NR44
    m_data[0xFF24] = 0x77; // NR50
    m_data[0xFF25] = 0xF3; // NR51
    m_data[0xFF26] = 0xF1; // NR52
    m_data[0xFF40] = 0x91; // LCDC
    m_data[0xFF42] = 0x00; // SCY
    m_data[0xFF43] = 0x00; // SCX
    m_data[0xFF45] = 0x00; // LYC
    m_data[0xFF47] = 0xFC; // BGP
    m_data[0xFF48] = 0xFF; // OBP0
    m_data[0xFF49] = 0xFF; // OBP1
    m_data[0xFF4A] = 0x00; // WY
    m_data[0xFF4B] = 0x00; // WX
    m_data[0xFFFF] = 0x00; // IE
  }

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

  byte *const not_usable_begin = std::data(m_data) + 0xFEA0U;
  byte *const not_usable_end = std::data(m_data) + 0xFEFFU;

  byte *const io_begin = std::data(m_data) + 0xFF00U;
  byte *const io_end = std::data(m_data) + 0xFF7FU;

  byte *const hram_begin = std::data(m_data) + 0xFF80U;
  byte *const hram_end = std::data(m_data) + 0xFFFEU;

public:
  std::shared_ptr<cartridge::cartridge> pCart;

private:
  mutable std::array<byte, 64 * 1024> m_data{};

  const byte bootrom[256] = {0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73,
                             0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F,
                             0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD,
                             0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
                             0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};
};

}

#endif

