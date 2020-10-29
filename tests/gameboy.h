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

  tmbl::ppu m_ppu{m_regs, m_cart, m_intr};
  tmbl::bus m_bus{m_cart, m_regs, m_intr, m_builtin, m_ppu};
  tmbl::cpu m_cpu{m_bus, m_regs, m_intr};
};

#endif
