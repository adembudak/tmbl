#pragma once
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
