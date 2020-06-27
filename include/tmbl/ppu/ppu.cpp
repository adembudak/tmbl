#include "ppu.h"

namespace tmbl::ppu {

[[maybe_unused]] bool ppu::lcdPower(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit7(f.value());
  }

  return LCDC.bit7();
}

[[maybe_unused]] bool ppu::windowTileMap(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit6(f.value());
  }

  return LCDC.bit6();
}

[[maybe_unused]] bool ppu::windowEnable(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit5(f.value());
  }

  return LCDC.bit5();
}

[[maybe_unused]] bool ppu::bgWindowTileSet(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit4(f.value());
  }

  return LCDC.bit4();
}

[[maybe_unused]] bool ppu::bgTileMap(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit3(f.value());
  }

  return LCDC.bit3();
}

[[maybe_unused]] bool ppu::bigSprite(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit2(f.value());
  }

  return LCDC.bit2();
}

[[maybe_unused]] bool ppu::spriteEnabled(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit1(f.value());
  }

  return LCDC.bit1();
}

[[maybe_unused]] bool ppu::bgEnabled(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    LCDC.bit0(f.value());
  }

  return LCDC.bit0();
}

}
