#ifndef PPU_H
#define PPU_H

#include "../config.h"
#include "../io/registers.h"
#include "internals/stat.h"
#include "internals/lcdc.h"
#include "internals/bgp.h"
#include "internals/obp.h"

#include <cstddef>
#include <array>
#include <functional>

namespace tmbl {

class cartridge;
class interrupts;

class ppu {
public:
  ppu(registers &regs_, cartridge &cart_, interrupts &intr_);

  static constexpr uint8 screenWidth = 160;
  static constexpr uint8 screenHeight = 144;

  struct color {
    uint8 r, g, b, a;
  };

  using palette = std::array<color, 5>; // 5th color for idle screen, i.e. when in LCDC.bit7 is 0

  void render(std::function<void(const uint8 x, const uint8 y, const color c)> draw,
              palette p = default_palette);

  byte readVRAM(const std::size_t index);
  void writeVRAM(const std::size_t index, const byte val);

  byte readOAM(const std::size_t index);
  void writeOAM(const std::size_t index, const byte val);

private:
  uint8 ly() const noexcept { return LY; }
  void ly(const byte val) noexcept {
    LY = val;
    LY == LYC ? STAT.match_flag(set) : STAT.match_flag(reset);
  }

  uint8 vbk() const noexcept { return VBK & 0b0000'0001 ? 1 : 0; }

private:
  registers &m_regs;
  cartridge &m_cart;
  interrupts &m_intr;

private:
  // see: https://archive.org/details/GameBoyProgManVer1.1/page/n16/mode/1up
  lcdc LCDC;
  stat STAT;

  byte &SCY;
  byte &SCX;

  byte &LY;
  byte &LYC;

  byte &DMA;
  bgp BGP;

  obp OBP0;
  obp OBP1;

  byte &WY;
  byte &WX;

  byte &VBK;

  byte &HDMA1;
  byte &HDMA2;
  byte &HDMA3;
  byte &HDMA4;
  byte &HDMA5;

  byte &BCPS;
  byte &BCPD;

  byte &OCPS;
  byte &OCPD;

  std::array<byte, 16_KB> m_vram{}; // it's 8KB on DMG
  std::array<byte, 160_B> m_oam{};

  uint8 tileWidth = 8;

  // clang-format off
  inline static palette default_palette{color{155, 188, 15, 0},  // light green
                                        color{139, 172, 15, 0},
                                        color{48, 98, 48, 0},
                                        color{15, 56, 15, 0},    // dark green
                                        color{161, 193, 19, 0}}; // idle screen color
};
}

#endif

