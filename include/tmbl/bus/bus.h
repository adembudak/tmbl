#ifndef BUS_H
#define BUS_H

#include "../config.h"

#include <array>
#include <memory>
#include <string>
#include <iterator>

namespace tmbl {

class reg8;
class reg16;
class cartridge;

class bus final {
public:
  // selects wram bank
  // 0-1 bank0
  // 1-7 bank1-bank7
  u8 SVBK() noexcept;

  // selects vram bank
  // 0 bank0
  // 1 bank1
  u8 VBK() noexcept;

  byte &IE() const noexcept;

  void plug(const cartridge &cart) noexcept;
  std::string title() const noexcept;

  [[nodiscard]] byte read(const reg8 r);
  [[nodiscard]] byte read(const reg16 rr);
  [[nodiscard]] byte read(const u8 n);
  [[nodiscard]] byte read(const u16 nn);

  void write(const reg8 r, const u8 n);
  void write(const reg16 rr, const reg8 r);
  void write(const reg16 rr, const u16 nn);
  void write(const u16 nn, const reg8 r);
  void write(const u16 nn, const byte b);
  void write(const reg16 rr, const byte b);
  void write(const reg16 rr1, const reg16 rr2);

  byte *const rom_bank0_begin = std::data(m_data) + 0x0000U;
  byte *const rom_bank0_end = std::data(m_data) + 0x3FFFU;

  byte *const rom_bankN_begin = std::data(m_data) + 0x4000U;
  byte *const rom_bankN_end = std::data(m_data) + 0x07FFFU;

  byte *const vram_begin = std::data(m_data) + 0x8000U;
  byte *const vram_end = std::data(m_data) + 0x9FFFU;

  byte *const sram_begin = std::data(m_data) + 0xA000U;
  byte *const sram_end = std::data(m_data) + 0xBFFFU;

  byte *const wram_bank0_begin = std::data(m_data) + 0xC000U;
  byte *const wram_bank0_end = std::data(m_data) + 0xCFFFU;

  byte *const wram_bankN_begin = std::data(m_data) + 0xD000U;
  byte *const wram_bankN_end = std::data(m_data) + 0xDFFFU;

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

  auto data() const noexcept { return m_data; }

  // Meyers' Singleton
  // see: More Effective C++ pp.131
  static bus &get() {
    static bus s;
    return s;
  }

private:
  bus();
  ~bus() = default;

  bus(const bus &other) = delete;
  bus &operator=(const bus &rhs) = delete;

  bus(bus &&other) = delete;
  bus &operator=(bus &&rhs) = delete;

private:
  byte read_byte(u16 index) const noexcept;
  void write_byte(u16 index, byte val) noexcept;

  mutable std::array<byte, 64 * 1024> m_data{};
  std::shared_ptr<cartridge> pCart;

  byte &reg_IE = m_data[0xFFFF];
  byte &reg_SVBK = m_data[0xFF70];
  byte &reg_VBK = m_data[0xFF4F];
};
}

#endif

