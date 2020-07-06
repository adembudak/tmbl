#ifndef PPU_H
#define PPU_H

#include "../config.h"
#include "../cpu/registers/reg8.h"
#include "../bus/bus.h"

#include <optional>
namespace tmbl {

class ppu final {

public:
  void tickFrame();

private:
  [[maybe_unused]] byte SCY(std::optional<byte> b) noexcept;
  [[maybe_unused]] byte SCX(std::optional<byte> b) noexcept;

  byte LY() const noexcept;
  void LYC(std::optional<byte> b) noexcept;

  [[maybe_unused]] byte WY(std::optional<byte> b) noexcept;
  [[maybe_unused]] byte WX(std::optional<byte> b) noexcept;

  // LCDC accessors
  // true: ldc is on
  // false: ldc is off
  // TODO: Create a type aliases for std::optional<flag>, what should be the name of it?
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

  // STAT accessors
  enum mode_flag : u8 {
    IN_HORIZONTAL_BLANKING_PERIOD = 0b00,
    IN_VERTICAL_BLANKING_PERIOD = 0b01,
    SEARCHING_OAM_RAM = 0b10,
    TRANSFERING_DATA_TO_LCD_DRIVER = 0b11
  };

  mode_flag mode() noexcept;
  flag match() const noexcept;
  [[maybe_unused]] bool coincidenceIRQ(std::optional<flag> f) noexcept;
  [[maybe_unused]] bool oamIRQ(std::optional<flag> f) noexcept;
  [[maybe_unused]] bool vblankIRQ(std::optional<flag> f) noexcept;
  [[maybe_unused]] bool hblankIRQ(std::optional<flag> f) noexcept;

private:
  bus &busInstance = bus::get();

  byte &reg_LCDC = busInstance.data().at(0xFF40); // LCD control
  byte &reg_STAT = busInstance.data().at(0xFF41); // LCD status

  byte &reg_SCY = busInstance.data().at(0xFF42); // scroll y axes
  byte &reg_SCX = busInstance.data().at(0xFF43); // scroll x axes

  byte &reg_LY = busInstance.data().at(0xFF44);  // layout
  byte &reg_LYC = busInstance.data().at(0xFF45); // layout control

  byte &reg_DMA = busInstance.data().at(0xFF46); // direct memory access
  byte &reg_BGP = busInstance.data().at(0xFF47); // background palette

  byte &reg_OBP0 = busInstance.data().at(0xFF48); // object (sprite) 0
  byte &reg_OBP1 = busInstance.data().at(0xFF49); // object (sprite) 1

  byte &reg_WY = busInstance.data().at(0xFF4A); // window pos. y
  byte &reg_WX = busInstance.data().at(0xFF4B); // window pos. x
};
}

#endif

