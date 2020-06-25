#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "config.h"
#include "cpu/cpu.h"
#include "bus/bus.h"

#include <filesystem>

namespace tmbl {

class Gameboy {
public:
  void plug(const std::filesystem::path &cart);

  void saveState();
  void loadState();
  void pause();
  void run();

  std::string title;

private:
  cpu::cpu c;
  bus::bus b;
};

}

#endif
