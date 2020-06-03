#ifndef CPU_H
#define CPU_H

#include "registers/reg16.h"
#include "registers/reg8.h"
#include "../memory/memory.h"
#include "../config.h"

namespace tmbl::cpu {

class cpu final {
public:
  void LD(reg8 &r, reg16 &rr, u8 CY) noexcept;
  void LD(reg16 &rr, reg8 &r, u8 CY) noexcept;
  void LD(reg16 &rr, u8 n, u8 CY) noexcept;
  void LD(reg8 &r1, reg8 &r2, u8 CY, int dummy) noexcept;
  void LD(reg8 &r1, reg8 &r2, u8 CY) noexcept;
  void LD(reg8 &r, u8 n, u8 CY, int dummy) noexcept;
  void LD(reg8 &r, u8 n, u8 CY) noexcept;
  void LD(u8 n, reg8 r, u8 CY) noexcept;
  void LD(reg8 &r, u16 nn, u8 CY) noexcept;
  void LD(u16 nn, reg8 &r, u8 CY) noexcept;
  void LD(u16 nn, reg16 &rr, u8 CY) noexcept;
  void LD(reg16 &rr1, reg16 &rr2, u8 CY) noexcept;
  void LD(reg16 &rr1, u16 nn, u8 CY) noexcept;

  void run();

private:
  byte fetch(reg16 rr) { return m[rr]; }
  int decode(byte b) { return std::to_integer<int>(b); }

private:
  mutable reg16 PC;
  mutable reg16 SP;

  mutable reg8 A;
  mutable reg8 F;

  mutable reg16 BC;
  mutable reg16 DE;
  mutable reg16 HL;

  memory::memory m;
};

}

#endif

