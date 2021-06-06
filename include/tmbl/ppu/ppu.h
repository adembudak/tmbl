#ifndef PPU_H
#define PPU_H

#include "tmbl/config.h"
#include "tmbl/clock/clock.h"
#include "tmbl/io/registers.h"
#include "internals/stat.h"
#include "internals/lcdc.h"

// dmg palettes
#include "internals/bgp.h"
#include "internals/obp.h"
// cgb palettes
#include "internals/bcps.h"
#include "internals/bcpd.h"
#include "internals/ocps.h"
#include "internals/ocpd.h"

#include "data/obj.h"

#include <cstddef>
#include <array>
#include <vector>
#include <functional>

namespace tmbl {

class cartridge;
class interrupts;

constexpr const uint8 screenWidth = 160;
constexpr const uint8 screenHeight = 144;

constexpr const uint8 screenVBlankHeight = 154; // n([0, 153]) = 154
constexpr const uint8 oamCycles = 20;
constexpr const uint8 vramCycles = 43;
constexpr const uint8 hblankCycles = 22;
constexpr const uint8 scanlineCycles = 114;

class ppu {
public:
  struct color {
    using subpixel = uint8;

    subpixel r{};
    subpixel g{};
    subpixel b{};
    uint8 a{}; // alpha

    friend bool operator==(const color &left, const color &right) {
      return left.r == right.r && left.g == right.g && left.b == right.b && left.a == right.a;
    }
  };

  using color_t = color;
  using palette_t = std::array<color_t, 5>; // 5th color for idle screen, i.e. LCDC.bit7 is 0

  using screenline = std::array<color_t, screenWidth>;
  using frame = std::array<screenline, screenHeight>;

  ppu(registers &regs_, cartridge &cart_, interrupts &intr_);
  void update(std::function<void(const tmbl::ppu::frame framebuffer)> drawCallback);

  byte readVRAM(const std::size_t index) const noexcept;
  void writeVRAM(const std::size_t index, [[maybe_unused]] const byte val) noexcept;

  byte readOAM(const std::size_t index) const noexcept;
  void writeOAM(const std::size_t index, [[maybe_unused]] const byte val) noexcept;

  void writeDMA(const byte val);

private:
  void fetchBackground() noexcept; // on bottom
  void fetchWindow() noexcept;
  void fetchSprite() noexcept; // on top
  void scanline() noexcept;

  uint8 vbk() const noexcept;

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

  byte &LY; // in range [0,153]. [144, 153] is vblank period
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

  std::vector<byte> m_vram{};
  std::array<byte, 160_B> m_oam{};
  frame framebuffer{};

  uint8 tileWidth = 8;
  uint8 tileHeight = 8;
  uint8 tileSize = 16_B;

  bool cgb_support = false;
  bool vram_accessible = false;
  bool oam_accessible = false;

  static constexpr palette_t default_palette{color{155, 188, 15, 255},  // light green
                                             color{139, 172, 15, 255},  //
                                             color{48, 98, 48, 255},    //
                                             color{15, 56, 15, 255},    // dark green
                                             color{161, 193, 19, 255}}; // idle screen color

  clock &m_clock = clock::instance(m_regs, m_intr);
};
}

#endif
