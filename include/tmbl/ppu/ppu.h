#ifndef PPU_H
#define PPU_H

#include "registers/reg8.h"
#include "clock/clock.h"
#include "../bus/bus.h"
#include "../config.h"
#include <optional>

#include <memory>

namespace tmbl::ppu {

class ppu final {

  explicit ppu(std::shared_ptr<bus::bus> b);
  [[maybe_unused]] bool lcdPower(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool windowTileMap(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool windowEnable(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bgWindowTileSet(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bgTileMap(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bigSprite(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool spriteEnabled(std::optional<flag> f = std::nullopt) noexcept;
  [[maybe_unused]] bool bgEnabled(std::optional<flag> f = std::nullopt) noexcept;

  byte LY() const noexcept { return bus->read(byte(0xFF44)); }

private:
  cpu::reg8 LCDC;
  cpu::reg8 STAT;
  std::shared_ptr<bus::bus> bus;
};
}

#endif

