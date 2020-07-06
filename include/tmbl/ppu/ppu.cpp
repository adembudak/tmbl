#include "ppu.h"

namespace tmbl {

// non-member bit manuplators for registers
// TODO: create a type alias for optional<flag>, what to name it???

flag bit0(byte &b, std::optional<flag> f = std::nullopt) noexcept;
flag bit1(byte &b, std::optional<flag> f = std::nullopt) noexcept;
flag bit2(byte &b, std::optional<flag> f = std::nullopt) noexcept;
flag bit3(byte &b, std::optional<flag> f = std::nullopt) noexcept;
flag bit4(byte &b, std::optional<flag> f = std::nullopt) noexcept;
flag bit5(byte &b, std::optional<flag> f = std::nullopt) noexcept;
flag bit6(byte &b, std::optional<flag> f = std::nullopt) noexcept;
flag bit7(byte &b, std::optional<flag> f = std::nullopt) noexcept;

byte ppu::SCY(std::optional<byte> b) noexcept {
  if (b.has_value()) {
    reg_SCX = b.value();
  }

  return reg_SCX;
}

byte ppu::SCX(std::optional<byte> b) noexcept {
  if (b.has_value())
    reg_SCY = b.value();

  return reg_SCY;
}

byte ppu::LY() const noexcept { return reg_LY; }
void ppu::LYC(std::optional<byte> b) noexcept {
  if (b.has_value())
    reg_LYC = b.value();

  if (reg_LYC == reg_LY)
    bit2(reg_STAT, set);
}

byte ppu::WY(std::optional<byte> b) noexcept {
  if (b.has_value())
    reg_WY = b.value();

  return reg_WY;
}

byte ppu::WX(std::optional<byte> b) noexcept {
  if (b.has_value())
    reg_WX = b.value();

  return reg_WX;
}

bool ppu::lcdPower(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit7(reg_LCDC, f.value());
  }

  return bit7(reg_LCDC);
}

bool ppu::windowTileMap(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit6(reg_LCDC, f.value());
  }

  return bit6(reg_LCDC);
}

bool ppu::windowEnable(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit5(reg_LCDC, f.value());
  }

  return bit5(reg_LCDC);
}

bool ppu::bgWindowTileSet(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit4(reg_LCDC, f.value());
  }

  return bit4(reg_LCDC);
}

bool ppu::bgTileMap(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit3(reg_LCDC, f.value());
  }

  return bit3(reg_LCDC);
}

bool ppu::bigSprite(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit2(reg_LCDC, f.value());
  }

  return bit2(reg_LCDC);
}

bool ppu::spriteEnabled(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit1(reg_LCDC, f.value());
  }

  return bit1(reg_LCDC);
}

bool ppu::bgEnabled(std::optional<flag> f) noexcept {
  if (f.has_value()) {
    bit0(reg_LCDC, f.value());
  }

  return bit0(reg_LCDC);
}

ppu::mode_flag ppu::mode() noexcept {
  if (lcdPower() == off) {
    return static_cast<mode_flag>(0);
  }

  return static_cast<mode_flag>(reg_STAT & 0b0000'0011);
}

bool ppu::coincidenceIRQ(std::optional<flag> f) noexcept {
  if (f.has_value())
    bit6(reg_STAT, f.value());

  return bit6(reg_STAT);
}

bool ppu::oamIRQ(std::optional<flag> f) noexcept {
  if (f.has_value())
    bit5(reg_STAT, f.value());

  return bit5(reg_STAT);
}

bool ppu::vblankIRQ(std::optional<flag> f) noexcept {
  if (f.has_value())
    bit4(reg_STAT, f.value());

  return bit4(reg_STAT);
}

bool ppu::hblankIRQ(std::optional<flag> f) noexcept {
  if (f.has_value())
    bit3(reg_STAT, f.value());

  return bit3(reg_STAT);
}

flag ppu::match() const noexcept { return reg_LY == reg_LYC; }

// non member bit manuplators for registers
flag bit0(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b0000'0001U : b &= 0b1111'1110U;
  }
  return (b & 0b0000'0001U) == 1 ? set : reset;
}

flag bit1(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b0000'0010U : b &= 0b1111'1101U;
  }
  return (((b & 0b0000'0010U) >> 1U) == 1) ? set : reset;
}

flag bit2(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b0000'0100 : b &= 0b1111'1011;
  }
  return ((b & 0b0000'0100U) >> 2U) == 1 ? set : reset;
}

flag bit3(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b0000'1000 : b &= 0b1111'0111;
  }
  return ((b & 0b0000'1000U) >> 3U) == 1 ? set : reset;
}

flag bit4(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b0001'0000 : b &= 0b1110'1111;
  }
  return ((b & 0b0001'0000U) >> 4U) == 1 ? set : reset;
}

flag bit5(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b0010'0000 : b &= 0b1101'1111;
  }
  return ((b & 0b0010'0000U) >> 5U) == 1 ? set : reset;
}

flag bit6(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b0100'0000 : b &= 0b1011'1111;
  }
  return ((b & 0b0100'0000U) >> 6U) == 1 ? set : reset;
}

flag bit7(byte &b, std::optional<flag> f) noexcept {
  if (f.has_value()) {
    f.value() == set ? b |= 0b1000'0000 : b &= 0b0111'1111;
  }
  return ((b & 0b1000'0000U) >> 7U) == 1 ? set : reset;
}
}
