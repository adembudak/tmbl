#ifndef CPU_H
#define CPU_H

#include "tmbl/bus/bus.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/clock/clock.h"

#include <memory>

namespace tmbl {

class cpu {
public:
  cpu(std::shared_ptr<bus> pBus, std::shared_ptr<registers> pReg, std::shared_ptr<interrupts> pIntr,
      std::shared_ptr<clock> pClock);

  void enableDoubleSpeedMode();

private:
  std::shared_ptr<bus> m_pBus;
  std::shared_ptr<clock> m_pClock;
  std::shared_ptr<registers> m_pReg;
  std::shared_ptr<interrupts> m_pIntr;

private:
  clock m_clock;

  flag IME = reset; // interrupt master enable
  byte &KEY1 = m_pReg->getAt(0xFF4D);
};

}

#endif
