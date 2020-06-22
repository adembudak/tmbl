#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "config.h"
#include "cpu/cpu.h"
#include "cartridge/cartridge.h"
#include "memory/memory.h"

#include <filesystem>

namespace tmbl {

class Gameboy {
public:
  void init(const std::filesystem::path &p);
  void saveState();
  void loadState();
  void pause();
  void run();

private:
  cpu::cpu c;
  cartridge::cartridge game;
};

}

#endif
