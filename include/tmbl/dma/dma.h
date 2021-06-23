#include "../config.h"

namespace tmbl {

class ppu;
class registers;
class builtin;
class cartridge;

class dma {
public:
  dma(ppu &ppu_, registers &regs_, builtin &builtin_, cartridge &cart_);

  void dmaAction() noexcept;

  void hdmaAction() noexcept;
  uint16 hdmaSource() const noexcept;
  uint16 hdmaDestination() const noexcept;

private:
  ppu &m_ppu;
  registers &m_regs;
  builtin &m_builtin;
  cartridge &m_cart;

private:
  byte &DMA;

  byte &HDMA1;
  byte &HDMA2;
  byte &HDMA3;
  byte &HDMA4;
  byte &HDMA5;
};

}
