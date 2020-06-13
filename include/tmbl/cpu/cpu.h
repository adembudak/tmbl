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

  void PUSH(const reg16 rr) noexcept;
  void POP(reg16 rr) noexcept;

  void ADD(const reg8 r) noexcept;
  void ADD(const u8 n) noexcept;
  void ADD(const reg16 rr) noexcept;

  void ADC(const reg8 r) noexcept;
  void ADC(const u8 n) noexcept;
  void ADC(const reg16 rr) noexcept;

  void SUB(const reg8 r) noexcept;
  void SUB(const u8 n) noexcept;
  void SUB(const reg16 rr) noexcept;

  void SBC(const reg8 r) noexcept;
  void SBC(const u8 n) noexcept;
  void SBC(const reg16 rr) noexcept;

  void AND(const reg8 r) noexcept;
  void AND(const u8 n) noexcept;
  void AND(const reg16 rr) noexcept;

  void OR(const reg8 r) noexcept;
  void OR(const u8 n) noexcept;
  void OR(const reg16 rr) noexcept;

  void XOR(const reg8 r) noexcept;
  void XOR(const u8 n) noexcept;
  void XOR(const reg16 rr) noexcept;

  void CP(const reg8 r) noexcept;
  void CP(const u8 n) noexcept;
  void CP(const reg16 rr) noexcept;

  void INC(reg8 r) noexcept;
  void INC(reg16 rr) noexcept;

  void DEC(reg8 r) noexcept;
  void DEC(reg16 rr) noexcept;

  void ADD(reg16 rr1, reg16 rr2) noexcept;
  void ADD(const u8 n, [[maybe_unused]] int dummy) noexcept;

  void INC(reg16 rr, [[maybe_unused]] int dummy) noexcept;
  void DEC(reg16 rr, [[maybe_unused]] int dummy) noexcept;

  void RLCA() noexcept;
  void RLA() noexcept;

  void RRCA() noexcept;
  void RRA() noexcept;

  void RLC(reg8 r) noexcept;
  void RLC(reg16 rr) noexcept;

  void RL(reg8 r) noexcept;
  void RL(reg16 rr) noexcept;

  void RRC(reg8 r) noexcept;
  void RRC(reg16 rr) noexcept;

  void RR(reg8 r) noexcept;
  void RR(reg16 rr) noexcept;

  void SLA(reg8 r) noexcept;
  void SLA(reg16 rr) noexcept;

  void SRA(reg8 r) noexcept;
  void SRA(reg16 rr) noexcept;

  void SRL(reg8 r) noexcept;
  void SRL(reg16 rr) noexcept;

  void SWAP(reg8 &r) noexcept;
  void SWAP(reg16 &rr) noexcept;

  void BIT(reg8 r, u8 pos);
  void BIT(reg16 rr, u8 pos);

  void SET(reg8 &r, u8 pos);
  void SET(reg16 rr, u8 pos);

  void RES(reg8 &r, u8 pos);
  void RES(reg16 rr, u8 pos);

  void JP(u16 nn) noexcept;
  void JP(u8 cc, u16 nn) noexcept;
  void JP(reg16 rr) noexcept;

  void JR(i8 e) noexcept;
  void JR(u8 cc, i8 e) noexcept;

  void CALL(u16 nn) noexcept;
  void CALL(u8 cc, u16 nn) noexcept;

  void RET() noexcept;
  void RETI() noexcept;
  void RET(u8 cc) noexcept;

  void RST(u8 t) noexcept;

  void DAA() noexcept;
  void CPL() noexcept;
  void NOP() noexcept;
  void CCF() noexcept;
  void SCF() noexcept;
  void DI() noexcept;
  void EI() noexcept;

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

  flag IME = false;
};
}

#endif

