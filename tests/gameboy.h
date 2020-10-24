#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "tmbl/bus/bus.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/cpu/cpu.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/registers.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <memory>
#include <filesystem>

class gameboy final {
public:
  void plugCart(std::filesystem::path p);
  void run();

private:
  tmbl::cartridge m_cart;
  tmbl::registers m_regs;
  tmbl::interrupts m_intr;
  tmbl::builtin m_builtin;

  tmbl::ppu m_ppu{std::make_shared<tmbl::registers>(m_regs),
                  std::make_shared<tmbl::cartridge>(m_cart),
                  std::make_shared<tmbl::interrupts>(m_intr)};

  tmbl::bus m_bus{std::make_shared<tmbl::cartridge>(m_cart),
                  std::make_shared<tmbl::registers>(m_regs),
                  std::make_shared<tmbl::interrupts>(m_intr),
                  std::make_shared<tmbl::builtin>(m_builtin), std::make_shared<tmbl::ppu>(m_ppu)};

  tmbl::cpu m_cpu{std::make_shared<tmbl::bus>(m_bus), std::make_shared<tmbl::registers>(m_regs),
                  std::make_shared<tmbl::interrupts>(m_intr)};
};

#endif
