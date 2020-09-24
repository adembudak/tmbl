#include "tmbl/io/timer/timer.h"

namespace tmbl {

class registers;

timer::timer(std::shared_ptr<registers> pReg) : m_pReg(pReg) {}

}

