#include "tmbl/config.h"
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

void cpu::adc(const r8 r) {
  uint c = F.c() == set ? 1 : 0;
  (A.loNibble() + r.loNibble() + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);

  A = A + r + c;
  A == 0b0000'0000 ? F.z(set) : F.z(reset);
  F.n(reset);
  (A.value() < r.value() + c) ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::adc(const byte b) {
  uint c = F.c() == set ? 1 : 0;
  (A.loNibble() + (b & 0b0000'1111) + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);
  A = A + b + c;

  A == 0b0000'0000 ? F.z(set) : F.z(reset);
  F.n(reset);
  (A.value() < b + c) ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::adc(const n8 n) {
  uint c = F.c() == set ? 1 : 0;
  (A.loNibble() + (n.value() & 0b0000'1111) + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);

  A = A + n.value() + c;

  A == 0b0000'0000 ? F.z(set) : F.z(reset);
  F.n(reset);

  (A.value() < n.value() + c) ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

}
