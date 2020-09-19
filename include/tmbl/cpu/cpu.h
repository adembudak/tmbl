#ifndef CPU_H
#define CPU_H

#include "tmbl/bus/bus.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/clock/clock.h"

#include "internals/flags.h"
#include "internals/n8.h"
#include "internals/n16.h"
#include "internals/r16.h"

#include <memory>

namespace tmbl {

class cpu {
public:
  cpu(std::shared_ptr<bus> pBus, std::shared_ptr<registers> pReg, std::shared_ptr<interrupts> pIntr,
      std::shared_ptr<clock> pClock);

  void enableDoubleSpeedMode();

private:
  void adc(const r8 r);
  void adc(const byte b);
  void adc(const n8 n);

private:
  std::shared_ptr<bus> m_pBus;
  std::shared_ptr<clock> m_pClock;
  std::shared_ptr<registers> m_pReg;
  std::shared_ptr<interrupts> m_pIntr;

private:
  r8 A;
  flags F;

  r16 BC;
  r16 DE;
  r16 HL;

  clock m_clock;
  flag IME = reset; // interrupt master enable
  byte &KEY1 = m_pReg->getAt(0xFF4D);
};
}

#endif
