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

#include <cstddef>
#include <array>
#include <vector>
#include <functional>

namespace tmbl {

class cartridge;
class interrupts;

constexpr const uint8 viewportWidth = 255;
constexpr const uint8 viewportHeight = 255;

constexpr const uint8 screenWidth = 160;
constexpr const uint8 screenHeight = 144;

constexpr const uint8 screenVBlankHeight = 154; // n([0, 153]) = 154
constexpr const uint8 oamCycles = 20;
constexpr const uint8 vramCycles = 43;
constexpr const uint8 hblankCycles = 22;
constexpr const uint8 scanlineCycles = 114;

class ppu final {
public:
  ppu(cartridge &cart_, registers &regs_, interrupts &intr_);

  // rgb555 pixel format, 5 bits per subpixel
  // can represent (2^5)^3 = 32'768 color
  // used in color gameboy, without backlight :(

  class color {
  public:
    color() = default;

    explicit color(const uint16 rgb555Val) : m_data(rgb555Val) {}
    color(const uint8 red, const uint8 green, const uint8 blue) {
      r(red);
      g(green);
      b(blue);
    }

    void r(const uint8 val) { m_data |= (0b0001'1111 & val); };
    void g(const uint8 val) { m_data |= ((0b0001'1111 & val) << 5); };
    void b(const uint8 val) { m_data |= ((0b0001'1111 & val) << 10); };

    friend bool operator==(const color &left, const color &right) {
      return left.m_data == right.m_data;
    }

  private:
    uint16 m_data = 0;
  };
  static_assert(sizeof(color) == sizeof(uint16));

  using palette_t = std::array<color, 4>;

  using screenline = std::array<color, screenWidth>;
  using frame = std::array<screenline, screenHeight>;

  void update(const std::function<void(tmbl::ppu::frame &framebuffer)> &drawCallback);
  byte readVRAM(const std::size_t index) const noexcept;
  void writeVRAM(const std::size_t index, [[maybe_unused]] const byte val) noexcept;

  byte readOAM(const std::size_t index) const noexcept;
  void writeOAM(const std::size_t index, [[maybe_unused]] const byte val) noexcept;

  statMode status() const noexcept;
  void adjustVRAMSize() noexcept;

private:
  void fetchBackground() noexcept; // on bottom
  void fetchWindow() noexcept;
  void fetchSprite() noexcept; // on top
  void scanline() noexcept;

  uint8 vbk() const noexcept;

  byte ly() const noexcept;
  void updateLY() noexcept;
  void resetLY() noexcept;

private:
  cartridge &m_cart;
  registers &m_regs;
  interrupts &m_intr;
  cflag &color_gameboy_support;

private:
  // https://archive.org/details/GameBoyProgManVer1.1/page/n16/mode/1up
  stat STAT;
  lcdc LCDC;

  byte &SCY;
  byte &SCX;

  byte &LY; // in range [0,153]. [144, 153] is vblank period
  byte &LYC;

  bgp BGP;

  obp OBP0;
  obp OBP1;

  byte &WY; // [0,143]
  byte &WX; // [7, 166]

  byte &VBK;

  bcps BCPS;
  byte &BCPD;

  byte &OCPS;
  byte &OCPD;

  std::vector<byte> m_vram;
  std::array<byte, 160_B> m_oam;
  frame m_frame;

  uint8 tileWidth = 8;
  uint8 tileHeight = 8;
  uint8 tileSize = 16_B;

  std::array<palette_t, 8> cgb_bg_palette{};
  std::array<palette_t, 8> cgb_sprite_palette{};

  const palette_t dmg_palette{color{1217},   // light green
                              color{5477},   //
                              color{17025},  //
                              color{19137}}; // dark green

  clock m_clock{m_regs, m_intr};
};
}

#endif
