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
  bus(std::shared_ptr<cartridge> pCart, std::shared_ptr<registers> pRegs,
      std::shared_ptr<interrupts> pIntr, std::shared_ptr<builtin> pBuiltin,
      std::shared_ptr<ppu> pPPU);

  byte readBus(const std::size_t index);
  void writeBus(std::size_t index, const byte val);

private:
  std::shared_ptr<cartridge> m_pCart;
  std::shared_ptr<registers> m_pRegs;
  std::shared_ptr<ppu> m_pPPU;
  std::shared_ptr<builtin> m_pBuiltin;
  std::shared_ptr<interrupts> m_pIntr;
};

}

#endif

