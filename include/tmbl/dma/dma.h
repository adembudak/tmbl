#include "tmbl/config.h"

namespace tmbl {

class ppu;
class registers;
class builtin;
class cartridge;

enum class hdmaStatus : uint8 { InProgress = 0, Done };

class dma {
public:
  dma(cartridge &cart_, ppu &ppu_, registers &regs_, builtin &builtin_);

  void dmaAction(const byte val) noexcept;
  void hdmaAction(const byte val) noexcept;

  hdmaStatus status() const noexcept;
  void status(const hdmaStatus st) noexcept;

private:
  uint16 hdmaSource() const noexcept;
  uint16 hdmaDestination() const noexcept;
  uint16 hdmaAmountOfBytesToTranfer() const noexcept;

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
