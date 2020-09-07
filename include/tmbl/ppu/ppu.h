#ifndef PPU_H
#define PPU_H

#include "../config.h"
#include "../io/registers.h"
#include "../io/interrupts/interrupts.h"
#include "../cartridge/cartridge.h"
#include "registers/stat.h"
#include "registers/lcdc.h"

#include <memory>
#include <array>
#include <utility>

namespace tmbl {

class ppu {
public:
  ppu(std::shared_ptr<registers> pRegs, std::shared_ptr<cartridge> pCart,
      std::shared_ptr<interrupts> pIntr);

  static constexpr n8 screenWidth = 160;
  static constexpr n8 screenHeight = 144;

  byte readVRAM(const std::size_t index);
  void writeVRAM(const std::size_t index, const byte val);

  byte readOAM(const std::size_t index);
  void writeOAM(const std::size_t index, const byte val);

private:
  n8 scx() const noexcept { return SCX; }
  n8 scy() const noexcept { return SCY; }

  n8 ly() const noexcept { return LY; }
  void ly(const byte val) noexcept {
    LY = val;
    LY == LYC ? STAT.match_flag(set) : STAT.match_flag(reset);
  }

  n8 vbk() const noexcept { return VBK & 0b0000'0001 ? 1 : 0; }

private:
  std::shared_ptr<registers> m_pRegs;
  std::shared_ptr<cartridge> m_pCart;
  std::shared_ptr<interrupts> m_pIntr;

private:
  // see: https://archive.org/details/GameBoyProgManVer1.1/page/n16/mode/1up
  lcdc LCDC;
  stat STAT;

  byte &SCY = m_pRegs->getAt(0xFF42);
  byte &SCX = m_pRegs->getAt(0xFF43);

  byte &LY = m_pRegs->getAt(0xFF44);
  byte &LYC = m_pRegs->getAt(0xFF45);

  byte &DMA = m_pRegs->getAt(0xFF46);
  byte &BGP = m_pRegs->getAt(0xFF47);

  byte &OBP0 = m_pRegs->getAt(0xFF48);
  byte &OBP1 = m_pRegs->getAt(0xFF49);

  byte &WY = m_pRegs->getAt(0xFF4A);
  byte &WX = m_pRegs->getAt(0xFF4B);

  byte &VBK = m_pRegs->getAt(0xFF4F);

  byte &HDMA1 = m_pRegs->getAt(0xFF51);
  byte &HDMA2 = m_pRegs->getAt(0xFF52);
  byte &HDMA3 = m_pRegs->getAt(0xFF53);
  byte &HDMA4 = m_pRegs->getAt(0xFF54);
  byte &HDMA5 = m_pRegs->getAt(0xFF55);

  byte &BCPS = m_pRegs->getAt(0xFF68);
  byte &BCPD = m_pRegs->getAt(0xFF69);

  byte &OCPS = m_pRegs->getAt(0xFF6A);
  byte &OCPD = m_pRegs->getAt(0xFF6B);

  std::array<byte, 16_KB> m_vram{}; // it's 8KB on DMG
  std::array<byte, 160_B> m_oam{};
};
}

#endif
