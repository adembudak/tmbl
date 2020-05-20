#pragma once
#include "../config.h"

namespace tmbl {

class cpu {
public:
  virtual ~cpu() = 0;
  static auto opcode = []() {};

private:
  reg16 PC:
};

}
