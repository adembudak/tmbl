#ifndef GAMEBOY_HPP
#define GAMEBOY_HPP

#include "config.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

namespace tmbl {

class Gameboy {
public:
  void saveState();
  void loadState();
  void pause();
  void run();
};

}

#endif
