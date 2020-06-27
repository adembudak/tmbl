#ifndef PPU_H
#define PPU_H

#include "../config.h"
#include "../cpu/registers/reg8.h"
#include "../bus/bus.h"

#include <optional>

namespace tmbl::ppu {

class ppu final {

  [[maybe_unused]] bool lcdPower(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool windowTileMap(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool windowEnable(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bgWindowTileSet(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bgTileMap(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bigSprite(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool spriteEnabled(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bgEnabled(std::optional<flag> f = std::nullopt) noexcept;

private:
  cpu::reg8 LCDC;
  cpu::reg8 STAT;
};
}

#endif

