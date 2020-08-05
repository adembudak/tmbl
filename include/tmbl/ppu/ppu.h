#ifndef PPU_H
#define PPU_H

#include "../config.h"
#include "../io/registers.h"
#include "../cartridge/cartridge.h"

#include <memory>
#include <array>

namespace tmbl {
class ppu {
public:
  ppu(std::shared_ptr<registers> pRegs, std::shared_ptr<cartridge> pCart)
      : m_pRegs(pRegs), m_pCart(pCart) {}

  // LCDC register functions
  // https://archive.org/details/GameBoyProgManVer1.1/page/n56/mode/1up
  cflag bgDisplayStatus() { return m_pCart->CGB() ? on : LCDC | 0b0000'0001; }
  cflag objStatus() { return LCDC & 0b0000'0010 ? on : off; }
  cflag objSizeBig() { return LCDC & 0b0000'0100 ? set : reset; }
  cflag bgCodeArea() { return LCDC & 0b0000'1000; }
  cflag bgChrArea() { return LCDC & 0b0001'0000; }
  cflag windowStatus() { return LCDC & 0b0010'0000 ? on : off; }
  cflag windowCodeArea() { return LCDC & 0b0100'0000; }
  cflag lcdControllerStatus() { return LCDC & 0b1000'0000 ? on : off; }

  // STAT register functions
  // https://archive.org/details/GameBoyProgManVer1.1/page/n57/mode/1up

  enum class mode : n8 {
    HORIZONTAL_BLANKING = 0b00,
    VERTICAL_BLANKING = 0b01,
    SEARCHING_OAM = 0b10,
    TRANSFERING_DATA_TO_LCD = 0b11,
  };

  mode mode_flag() {
    switch (STAT | 0b0000'0011) {
      case 0:
        return mode::HORIZONTAL_BLANKING;
      case 1:
        return mode::VERTICAL_BLANKING;
      case 2:
        return mode::SEARCHING_OAM;
      case 3:
        return mode::TRANSFERING_DATA_TO_LCD;
    }
  }

  void mode_flag(const mode m) {
    switch (m) {
      case mode::HORIZONTAL_BLANKING:
        STAT |= 0b00;
      case mode::VERTICAL_BLANKING:
        STAT |= 0b01;
      case mode::SEARCHING_OAM:
        STAT |= 0b10;
      case mode::TRANSFERING_DATA_TO_LCD:
        STAT |= 0b11;
    }
  }

  cflag match_flag() { return LY == LYC; }
  void match_flag(cflag val) { val ? STAT |= 0b0000'0100 : STAT &= 0b1111'1011; }

  // interrupts
  cflag matchHblank() { return STAT & 0b0000'1000; }
  cflag matchVblank() { return STAT & 0b0001'0000; }
  cflag matchSearchOAM() { return STAT & 0b0010'0000; }
  cflag matchCoincidence() { return STAT & 0b0100'0000; }

  n8 scx() const noexcept { return SCX; }
  n8 scy() const noexcept { return SCY; }

  n8 ly() const noexcept { return LY; }
  void ly(const byte val) noexcept {
    LY = val;
    LY == LYC ? match_flag(set) : match_flag(reset);
  }

private:
  std::shared_ptr<registers> m_pRegs;
  std::shared_ptr<cartridge> m_pCart;

private:
  // see: https://archive.org/details/GameBoyProgManVer1.1/page/n16/mode/1up

  byte &LCDC = m_pRegs->getAt(0xFF40);
  byte &STAT = m_pRegs->getAt(0xFF41);

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

  byte &HDMA1 = m_pRegs->getAt(0xFF51);
  byte &HDMA2 = m_pRegs->getAt(0xFF52);
  byte &HDMA3 = m_pRegs->getAt(0xFF53);
  byte &HDMA4 = m_pRegs->getAt(0xFF54);
  byte &HDMA5 = m_pRegs->getAt(0xFF55);

  byte &BCPS = m_pRegs->getAt(0xFF68);
  byte &BCPD = m_pRegs->getAt(0xFF69);

  byte &OCPS = m_pRegs->getAt(0xFF6A);
  byte &OCPD = m_pRegs->getAt(0xFF6B);

  std::array<byte, 8_KB> m_vram{};
  std::array<byte, 160_B> m_oam{};
};
}

#endif
