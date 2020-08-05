#ifndef TIMER_H
#define TIMER_H

#include "../registers.h"
#include <memory>

namespace tmbl {

class timer {
public:
  timer(std::shared_ptr<registers> pReg);

private:
  std::shared_ptr<registers> m_pReg;

private:
  byte &DIV = m_pReg->getAt(0xFF04);
  byte &TIMA = m_pReg->getAt(0xFF05);
  byte &TMA = m_pReg->getAt(0xFF06);
  byte &TAC = m_pReg->getAt(0xFF07);
};
}

#endif

