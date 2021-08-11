#include "tmbl/ppu/internals/lcdc.h"

namespace tmbl {

lcdc::lcdc(byte &val_, cflag &cgb_support) : m_value(val_), color_gameboy_support(cgb_support) {}

flag lcdc::lcdControllerStatus() const noexcept { return m_value & 0b1000'0000 ? on : off; }

std::pair<uint16, uint16> lcdc::winTilemapSelect() const noexcept {
  return m_value & 0b0100'0000 ? std::make_pair(0x1C00, 0x1FFF) : std::make_pair(0x1800, 0x1BFF);
}

flag lcdc::winDisplayStatus() const noexcept { return m_value & 0b0010'0000 ? on : off; }

std::pair<uint16, bool> lcdc::tilesetBasePtr() const noexcept {
  return m_value & 0b0001'0000 ? std::make_pair(0x0000, false) : std::make_pair(0x1000, true);
}

std::pair<uint16, uint16> lcdc::bgTilemapSelect() const noexcept {
  return m_value & 0b0000'1000 ? std::make_pair(0x1C00, 0x1FFF) : std::make_pair(0x1800, 0x1BFF);
}

uint8 lcdc::spriteHeight() const noexcept { return m_value & 0b0000'0100 ? 16 : 8; }

flag lcdc::objDisplayStatus() const noexcept { return m_value & 0b0000'0010 ? on : off; }

flag lcdc::bgDisplayStatus() const noexcept {
  return color_gameboy_support ? on : m_value & 0b0000'0001;
}

}
