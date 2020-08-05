#include "tmbl/cpu/cpu.h"

namespace tmbl {
cpu::cpu(std::shared_ptr<bus> pBus, std::shared_ptr<registers> pReg,
         std::shared_ptr<interrupts> pIntr, std::shared_ptr<clock> pClock)
    : m_pBus(pBus), m_pReg(pReg), m_pIntr(pIntr), m_pClock(pClock) {}

// see: double speed mode switch procedure
// https://archive.org/details/GameBoyProgManVer1.1/page/n35/mode/1up

void cpu::enableDoubleSpeedMode() {
  if ((KEY1 & 0b0100'0000) && (KEY1 & 0b0000'0001)) {
    m_pClock->enableDoubleSpeedMode(true);
    m_pReg->getAt(0xFF0F) = 0; // reset IF
    m_pIntr->IE = 0;
    m_pReg->getAt(0xFF00) |= 0x0011'0000;
    // STOP();
  }
}

}
