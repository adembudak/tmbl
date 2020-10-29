#include "tmbl/io/timer/timer.h"

namespace tmbl {

class registers;

timer::timer(registers &reg_)
    : m_reg(reg_), DIV(m_reg.getAt(0xFF04)), TIMA(m_reg.getAt(0xFF05)), TMA(m_reg.getAt(0xFF06)),
      TAC(m_reg.getAt(0xFF07)) {}
}

