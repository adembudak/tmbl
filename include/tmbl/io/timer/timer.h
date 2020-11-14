#ifndef TIMER_H
#define TIMER_H

#include "tmbl/config.h"
#include "../registers.h"

namespace tmbl {

class timer {
public:
  explicit timer(registers &reg_);

private:
  registers &m_reg;

private:
  byte &DIV;
  byte &TIMA;
  byte &TMA;
  byte &TAC;
};
}

#endif

