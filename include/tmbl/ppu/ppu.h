#ifndef PPU_H
#define PPU_H

#include "../config.h"
#include "../cpu/registers/reg8.h"
#include "../bus/bus.h"

#include <optional>
namespace tmbl {

class ppu final {

private:
  byte SCY() { return busInstance.data().at(0xFF42); }
  byte SCX() { return busInstance.data().at(0xFF43); }
  byte LY() const { return busInstance.data().at(0xFF44); }

  byte LYC();

  byte &WX() noexcept;
  byte &WY() noexcept;

  // true: ldc is on
  // false: ldc is off
  [[maybe_unused]] bool lcdPower(std::optional<flag> f = std::nullopt) noexcept;

  // true: 9C00-9FFF
  // false: 9800-9BFF
  [[maybe_unused]] bool windowTileMap(std::optional<flag> f = std::nullopt) noexcept;

  // true: window is on
  // false: window is off
  [[maybe_unused]] bool windowEnable(std::optional<flag> f = std::nullopt) noexcept;

  // true: 8000-8FFF
  // false: 8800-97FF
  [[maybe_unused]] bool bgWindowTileSet(std::optional<flag> f = std::nullopt) noexcept;

  // true: 9C00-9FFF
  // false: 9800-9BFF
  [[maybe_unused]] bool bgTileMap(std::optional<flag> f = std::nullopt) noexcept;

  // true: 8x16 dots
  // false: 8x8 dots
  [[maybe_unused]] bool bigSprite(std::optional<flag> f = std::nullopt) noexcept;

  // true: on
  // false: off
  [[maybe_unused]] bool spriteEnabled(std::optional<flag> f = std::nullopt) noexcept;

  // In CGB, always true
  // In DMG, true: on, false off
  [[maybe_unused]] bool bgEnabled(std::optional<flag> f = std::nullopt) noexcept;

private:
  reg8 LCDC;
  reg8 STAT;

  bus &busInstance = bus::get();
};
}

#endif

