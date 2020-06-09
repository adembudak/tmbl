#ifndef CPU_H
#define CPU_H

#include "registers/reg16.h"
#include "registers/reg8.h"
#include "clock/clock.h"
#include "../memory/memory.h"
#include "../config.h"

namespace tmbl::cpu {

class cpu final {
public:
  void LD(reg8 &r, reg16 &rr, u8 CY) noexcept;
  void LD(reg16 &rr, reg8 &r, u8 CY) noexcept;
  void LD(reg16 &rr, u8 n, u8 CY) noexcept;
  void LD(reg8 &r1, reg8 &r2, u8 CY, [[maybe_unused]] int dummy) noexcept;
  void LD(reg8 &r1, reg8 &r2, u8 CY) noexcept;
  void LD(reg8 &r, u8 n, u8 CY, [[maybe_unused]] int dummy) noexcept;
  void LD(reg8 &r, u8 n, u8 CY) noexcept;
  void LD(u8 n, reg8 r, u8 CY) noexcept;
  void LD(reg8 &r, u16 nn, u8 CY) noexcept;
  void LD(u16 nn, reg8 &r, u8 CY) noexcept;
  void LD(u16 nn, reg16 &rr, u8 CY) noexcept;
  void LD(reg16 &rr1, reg16 &rr2, u8 CY) noexcept;
  void LD(reg16 &rr1, u16 nn, u8 CY) noexcept;

  void PUSH(const reg16 rr);
  void POP(reg16 rr);

  void ADD(const reg8 r);
  void ADD(const u8 n);
  void ADD(const reg16 rr);

  void ADC(const reg8 r);
  void ADC(const u8 n);
  void ADC(const reg16 rr);

  void SUB(const reg8 r);
  void SUB(const u8 n);
  void SUB(const reg16 rr);

  void SBC(const reg8 r);
  void SBC(const u8 n);
  void SBC(const reg16 rr);

  void AND(const reg8 r);
  void AND(const u8 n);
  void AND(const reg16 rr);

  void OR(const reg8 r);
  void OR(const u8 n);
  void OR(const reg16 rr);

  void XOR(const reg8 r);
  void XOR(const u8 n);
  void XOR(const reg16 rr);

  void CP(const reg8 r);
  void CP(const u8 n);
  void CP(const reg16 rr);

  void INC(reg8 r);
  void INC(reg16 rr);

  void DEC(reg8 r);
  void DEC(reg16 rr);

  void ADD(reg16 rr1, reg16 rr2);
  void ADD(const u8 n, [[maybe_unused]] int dummy);

  void INC(reg16 rr, [[maybe_unused]] int dummy);
  void DEC(reg16 rr, [[maybe_unused]] int dummy);

  void RLCA();
  void RLA();

  void RRCA();
  void RRA();

  void RLC(reg8 r);
  void RLC(reg16 rr);

  void RL(reg8 r);
  void RL(reg16 rr);

  void RRC(reg8 r);
  void RRC(reg16 rr);

  void RR(reg8 r);
  void RR(reg16 rr);

  void SLA(reg8 r);
  void SLA(reg16 rr);

  void SRA(reg8 r);
  void SRA(reg16 rr);

  void SRL(reg8 r);
  void SRL(reg16 rr);

  void SWAP(reg8 &r);
  void SWAP(reg16 &rr);

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
  clock c;
};

}

#endif

