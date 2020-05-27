#ifndef CPU_H
#define CPU_H

#include "registers/reg16.h"
#include "registers/reg8.h"

namespace tmbl::cpu {

class cpu final {
public:
  //  int fetch();
  // void decode();
  // void execute();

private:
  // 8 bit transfer and I/O instructions
  //
  void LD(reg8 r1, reg8 r2) noexcept;
  void LD(reg8 r, uint8 n) noexcept;
  void LD(reg8 r1, reg16 r2) noexcept;
  void LD(reg16 r1, reg8 r2) noexcept;
  void LD(reg16 r, uint8 n) noexcept;
  // void LD(reg8 r, C) // port register or mode register in ram

private:
  mutable reg16 PC;
  mutable reg16 SP;

  mutable reg8 A;
  mutable reg8 F;

  mutable reg16 BC;
  mutable reg16 DE;
  mutable reg16 HL;
};

static_assert(is_regular_v<cpu>);
}

#endif

