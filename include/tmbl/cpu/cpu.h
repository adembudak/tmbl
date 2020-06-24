#ifndef CPU_H
#define CPU_H

#include "registers/reg16.h"
#include "registers/reg8.h"
#include "clock/clock.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "../config.h"

namespace tmbl::cpu {

class cpu final {
public:
  void run();

private:
  void LD(reg8 &r1, const reg8 r2) noexcept;
  void LD(reg8 &r, const u8 n) noexcept;
  void LD(reg8 &r, const reg16 rr) noexcept;

  void LD(const reg16 rr, const reg8 r) noexcept;

  void LDi() noexcept;
  void LDi([[maybe_unused]] int dummy) noexcept;

  void LDd() noexcept;
  void LDd([[maybe_unused]] int dummy) noexcept;

  void LD(const u16 nn) noexcept;
  void LD(const u16 nn, [[maybe_unused]] int dummy) noexcept;

  enum class Orientation { READ_FROM_IO_PORT, WRITE_TO_IO_PORT };
  void LD(const u8 n, Orientation o) noexcept;
  void LD(Orientation o) noexcept;

  void LD(const reg16 rr, const u8 n, [[maybe_unused]] int dummy) noexcept;
  void LD(reg16 &rr, const u16 nn) noexcept;
  void LD() noexcept;
  void LD(const i8 e) noexcept;
  void LD(const u16 nn, const reg16 rr) noexcept;

  void PUSH(const reg16 rr) noexcept;
  void POP(reg16 &rr) noexcept;

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

  void INC(reg8 &r) noexcept;
  void INC() noexcept;

  void DEC(reg8 &r) noexcept;
  void DEC() noexcept;

  void ADD(reg16 &rr1, const reg16 rr2) noexcept;
  void ADD(reg16 &rr, const i8 e) noexcept;

  void INC(reg16 &rr) noexcept;
  void DEC(reg16 &rr) noexcept;

  void RLCA() noexcept;
  void RLA() noexcept;

  void RRCA() noexcept;
  void RRA() noexcept;

  void RLC(reg8 &r) noexcept;
  void RLC(const reg16 rr) noexcept;

  void RL(reg8 &r) noexcept;
  void RL(const reg16 rr) noexcept;

  void RRC(reg8 &r) noexcept;
  void RRC(const reg16 rr) noexcept;

  void RR(reg8 &r) noexcept;
  void RR(const reg16 rr) noexcept;

  void SLA(reg8 &r) noexcept;
  void SLA(const reg16 rr) noexcept;

  void SRA(reg8 &r) noexcept;
  void SRA(const reg16 rr) noexcept;

  void SRL(reg8 &r) noexcept;
  void SRL(const reg16 rr) noexcept;

  void SWAP(reg8 &r) noexcept;
  void SWAP(const reg16 rr) noexcept;

  void BIT(const reg8 r, const u8 pos);
  void BIT(const reg16 rr, const u8 pos);

  void SET(reg8 &r, const u8 pos);
  void SET(const reg16 rr, const u8 pos);

  void RES(reg8 &r, const u8 pos);
  void RES(const reg16 rr, const u8 pos);

  void JP(const u16 nn) noexcept;
  void JP(const u8 cc, const u16 nn) noexcept;
  void JP(const reg16 rr) noexcept;

  void JR(const i8 e) noexcept;
  void JR(const u8 cc, const i8 e) noexcept;

  void CALL(const u16 nn) noexcept;
  void CALL(const u8 cc, const u16 nn) noexcept;

  void RET() noexcept;
  void RETI() noexcept;
  void RET(const u8 cc) noexcept;

  void RST(const u8 t) noexcept;

  void DAA() noexcept;
  void CPL() noexcept;
  void NOP() noexcept;
  void CCF() noexcept;
  void SCF() noexcept;
  void DI() noexcept;
  void EI() noexcept;

  void HALT() noexcept;
  void STOP() noexcept;

  template <typename T> T rotl(T x, int s) noexcept;
  template <typename T> T rotr(T x, int s) noexcept;

private:
  byte fetch(reg16 rr);

private:
  mutable reg16 PC;
  mutable reg16 SP;

  mutable reg8 A;
  mutable reg8 F;

  mutable reg16 BC;
  mutable reg16 DE;
  mutable reg16 HL;

  std::shared_ptr<bus::bus> bus;
  clock c;

  flag IME = false;
};
}

#endif

