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
  std::shared_ptr<tmbl::cartridge> p_cart = std::make_shared<tmbl::cartridge>();
  std::shared_ptr<tmbl::registers> p_regs = std::make_shared<tmbl::registers>();
  std::shared_ptr<tmbl::interrupts> p_intr = std::make_shared<tmbl::interrupts>();
  std::shared_ptr<tmbl::builtin> p_builtin = std::make_shared<tmbl::builtin>();

  tmbl::ppu m_ppu{p_regs, p_cart, p_intr};
  tmbl::bus m_bus{p_cart, p_regs, p_intr, p_builtin, std::make_shared<tmbl::ppu>(m_ppu)};
  tmbl::cpu m_cpu{std::make_shared<tmbl::bus>(m_bus), p_regs, p_intr};
};

#endif
