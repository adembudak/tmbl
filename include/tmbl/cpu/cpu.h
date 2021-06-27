#ifndef CPU_H
#define CPU_H

#include "tmbl/bus/bus.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/clock/clock.h"

#include "internals/flags.h"
#include "internals/n8.h"
#include "internals/e8.h"
#include "internals/n16.h"
#include "internals/r16.h"

#include <array>

namespace tmbl {

class cpu {
public:
  cpu(bus &bus_, registers &reg_, interrupts &intr_);

  void enableDoubleSpeedMode();

  enum class cc;
  void run();

private:
  struct tag {};
  void adc(const r8 r);
  void adc(const byte b);
  void adc(const n8 n);

  void add(const r8 r);
  void add(const byte b);
  void add(const n8 n);
  void add(const e8 e);
  void add(const r16 rr);

  void and_(const r8 r); // and is a reserved word so, and_
  void and_(const byte b);
  void and_(const n8 n);

  void cp(const r8 r);
  void cp(const byte b);
  void cp(const n8 n);

  void dec(r8 &r);
  void dec(const uint16 uu);
  void dec(r16 &rr);

  void inc(r8 &r);
  void inc(const uint16 uu);
  void inc(r16 &rr);

  void or_(const r8 r);
  void or_(const byte b);
  void or_(const n8 n);

  void sbc(const r8 r);
  void sbc(const byte b);
  void sbc(const n8 n);

  void sub(const r8 r);
  void sub(const byte b);
  void sub(const n8 n);

  void xor_(const r8 r);
  void xor_(const byte b);
  void xor_(const n8 n);

  void bit(const uint8 pos, const r8 r);
  void bit(const uint8 pos, const byte b);

  void res(const uint8 pos, r8 &r);
  void res(const uint8 pos, const uint16 uu);

  void set_(const uint8 pos, r8 &r);
  void set_(const uint8 pos, const uint16 uu);

  void swap(r8 &r);
  void swap(const uint16 uu);

  void rl(r8 &r);
  void rl(const uint16 uu);

  void rla();

  void rlc(r8 &r);
  void rlc(const uint16 uu);

  void rlca();

  void rr(r8 &r);
  void rr(const uint16 uu);

  void rra();

  void rrc(r8 &r);
  void rrc(const uint16 uu);

  void rrca();

  void sla(r8 &r);
  void sla(const uint16 uu);

  void sra(r8 &r);
  void sra(const uint16 uu);

  void srl(r8 &r);
  void srl(const uint16 uu);

  void ld(r8 &l, const r8 &r);
  void ld(r8 &r, const n8 &n);
  void ld(const uint16 uu, const r8 &r);
  void ld(r8 &r, const uint16 uu);

  void ldi(r16 &rr, const r8 r);
  void ldi(r8 &r, r16 &rr);

  void ldd(r16 &rr, const r8 r);
  void ldd(r8 &r, r16 &rr);

  void ld(r16 &rr, const n16 nn);

  void ld(const n16 nn, tag dummy);
  void ld(tag dummy, const n16 nn);

  void ldio(const n8 n, const r8 &r);
  void ldio(r8 &r, const n8 n);

  void ldio(const n8 n, const r8 &r, tag);
  void ldio(r8 &r, const n8 n, tag);

  void call(n16 nn);
  void call(cc c, n16 nn);

  void jp();
  void jp(const n16 nn);
  void jp(const cc c, const n16 nn);

  void jr(const e8 e);
  void jr(const cc c, const e8 e);

  void ret();
  void ret(cc c);
  void reti();

  void rst(const uint8 u);

  void pop();
  void pop(r16 &rr);

  void push();
  void push(const r16 &rr);

  void ccf();
  void cpl();

  void daa();

  void di();
  void ei();

  void halt();
  void nop();
  void scf();
  void stop();

private:
  void serveInterrupts() noexcept;

private:
  bus &m_bus;
  registers &m_regs;
  interrupts &m_intr;

private:
  r8 A;
  flags F;

  r16 BC;
  r16 DE;
  r16 HL;

  uint16 PC;
  uint16 SP;

  const std::array<int, 8> rst_vec{0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38};
  const std::array<int, 5> intr_vec{0x40, 0x48, 0x50, 0x58, 0x60};

  clock m_clock{m_regs, m_intr};
  flag IME = reset; // interrupt master enable
  byte &KEY1;
};
}

#endif
