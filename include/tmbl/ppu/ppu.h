#ifndef PPU_H
#define PPU_H

#include "../config.h"
#include "../io/registers.h"
#include "../cartridge/cartridge.h"

#include <memory>
#include <array>
#include <utility>

namespace tmbl {
class ppu {
public:
  ppu(std::shared_ptr<registers> pRegs, std::shared_ptr<cartridge> pCart);
  enum class mode : n8 {
    HORIZONTAL_BLANKING = 0b00,
    VERTICAL_BLANKING = 0b01,
    SEARCHING_OAM = 0b10,
    TRANSFERING_DATA_TO_LCD = 0b11,
  };

private:
  // LCDC register functions begin
  // https://archive.org/details/GameBoyProgManVer1.1/page/n56/mode/1up
  cflag bgDisplayStatus() const noexcept;
  cflag objDisplayStatus() const noexcept;
  std::pair<n8, n8> objSize() const noexcept;
  std::pair<n16, n16> bgCodeArea() const noexcept;
  std::pair<n16, n16> bgChrArea() const noexcept;
  cflag windowStatus() const noexcept;
  std::pair<n16, n16> windowCodeArea() const noexcept;
  cflag lcdControllerStatus() const noexcept;
  // LCDC register functions end

  // STAT register functions begin
  // https://archive.org/details/GameBoyProgManVer1.1/page/n57/mode/1up

  mode mode_flag() const noexcept;
  void mode_flag(const mode m) noexcept;

  cflag match_flag() const noexcept;
  void match_flag(cflag val) noexcept;
  // STAT register functions end

  // interrupts
  cflag matchHblank() { return STAT & 0b0000'1000; }
  cflag matchVblank() { return STAT & 0b0001'0000; }
  cflag matchSearchOAM() { return STAT & 0b0010'0000; }
  cflag matchCoincidence() { return STAT & 0b0100'0000; }
  // STAT register functions end

  n8 scx() const noexcept { return SCX; }
  n8 scy() const noexcept { return SCY; }

  n8 ly() const noexcept { return LY; }
  void ly(const byte val) noexcept {
    LY = val;
    LY == LYC ? match_flag(set) : match_flag(reset);
  }

  n8 vbk() const noexcept { return VBK & 0b0000'0001 ? 1 : 0; }

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
