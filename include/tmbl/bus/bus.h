#ifndef BUS_H
#define BUS_H

#include "tmbl/config.h"
#include <cstddef>

namespace tmbl {

class builtin;
class cartridge;
class interrupts;
class ppu;
class registers;
class dma;

class bus {
public:
  bus(cartridge &cart_, registers &regs_, interrupts &intr_, builtin &builtin_, ppu &ppu_,
      dma &dma_);

  byte readBus(const std::size_t index) const noexcept;
  void writeBus(const std::size_t index, const byte val) noexcept;

private:
  cartridge &m_cart;
  registers &m_regs;
  ppu &m_ppu;
  builtin &m_builtin;
  interrupts &m_pintr;
  dma &m_dma;
};

}

#endif
