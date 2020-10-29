#ifndef BUS_H
#define BUS_H

#include "../config.h"
#include <cstddef>
#include <memory>

namespace tmbl {

class builtin;
class cartridge;
class interrupts;
class ppu;
class registers;

class bus {
public:
  bus(cartridge &pCart, registers &pRegs, interrupts &pIntr, builtin &pBuiltin, ppu &pPPU);

  byte readBus(const std::size_t index);
  void writeBus(const std::size_t index, const byte val);

private:
  cartridge &m_cart;
  registers &m_regs;
  ppu &m_PPU;
  builtin &m_builtin;
  interrupts &m_pintr;
};

}

#endif

