#include "tmbl/io/timer/timer.h"

namespace tmbl {

class registers;

timer::timer(std::shared_ptr<registers> pReg)
    : m_pReg(pReg), DIV(m_pReg->getAt(0xFF04)), TIMA(m_pReg->getAt(0xFF05)),
      TMA(m_pReg->getAt(0xFF06)), TAC(m_pReg->getAt(0xFF07)) {}

}

