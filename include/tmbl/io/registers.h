#ifndef REGISTERS_H
#define REGISTERS_H

#include "../config.h"
#include <array>

namespace tmbl {

// A class to hold registers:
// see: https://archive.org/details/GameBoyProgManVer1.1/page/n16/mode/1up

class registers {
public:
  byte &getAt(const std::size_t index);
  void write(const std::size_t index, const byte val);

private:
  std::array<byte, 160_B> regs{};
};

}

#endif
