#ifndef TIMER_H
#define TIMER_H

#include "tmbl/config.h"
#include "../registers.h"
#include <memory>

namespace tmbl {

class timer {
public:
  timer(std::shared_ptr<registers> pReg);

private:
  std::shared_ptr<registers> m_pReg;

private:
  byte &DIV;
  byte &TIMA;
  byte &TMA;
  byte &TAC;
};
}

#endif

