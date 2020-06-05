#include "cpu.h"

namespace tmbl::cpu {

void cpu::LD(reg8 &r, reg16 &rr, u8 CY) noexcept {
  r = m[rr];
  PC += CY;
}

void cpu::LD(reg16 &rr, reg8 &r, u8 CY) noexcept {
  m[rr] = byte(r.data());
  PC += CY;
}

void cpu::LD(reg16 &rr, u8 n, u8 CY) noexcept {
  m[rr] = byte(n);
  PC += CY;
}

void cpu::LD(reg8 &r1, reg8 &r2, u8 CY, [[maybe_unused]] int dummy) noexcept {
  m[r1] = byte(r2.data());
  PC += CY;
}

void cpu::LD(reg8 &r1, reg8 &r2, u8 CY) noexcept {
  r1 = r2;
  PC += CY;
}

void cpu::LD(reg8 &r, u8 n, u8 CY, [[maybe_unused]] int dummy) noexcept {
  r = m[n];
  PC += CY;
}

void cpu::LD(reg8 &r, u8 n, u8 CY) noexcept {
  r = n;
  PC += CY;
}

void cpu::LD(u8 n, reg8 r, u8 CY) noexcept {
  m[n] = byte(r.data());
  PC += CY;
}

void cpu::LD(reg8 &r, u16 nn, u8 CY) noexcept {
  r = m[nn];
  PC += CY;
}

void cpu::LD(u16 nn, reg8 &r, u8 CY) noexcept {
  m[nn] = byte(r.data());
  PC += CY;
}

void cpu::LD(u16 nn, reg16 &rr, u8 CY) noexcept {
  m[nn] = byte(rr.data());
  PC += CY;
}

void cpu::LD(reg16 &rr1, reg16 &rr2, u8 CY) noexcept {
  rr1 = rr2;
  PC += CY;
}

void cpu::LD(reg16 &rr1, u16 nn, u8 CY) noexcept {
  rr1 = nn;
  PC += CY;
}

void cpu::PUSH(const reg16 rr) {
  m[SP - 1] = rr.lo();
  m[SP - 2] = rr.hi();
  SP -= 2;

  PC += 4;
}

void cpu::POP(reg16 rr) {
  rr.lo(m[SP]);
  rr.hi(m[SP + 1]);
  SP += 2;

  PC += 3;
}
void cpu::ADD(const reg8 r) {
  ((r.data() + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((r.loNibble() + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (r.data() + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  A = A + r;

  PC += 1;
}

void cpu::ADD(const u8 n) {
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A + tmp;

  PC += 1;
}

void cpu::ADD(const reg16 rr) {
  u8 n = std::to_integer<int>(m[rr]);
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];
  A = A + tmp;

  PC += 2;
}

void cpu::run() {
  for (;;) {

    //    switch (byte addr = fetch(); decode(addr)) {
    //    case 0x00:
    //     NOP();
    //   case 0x01:
  }
};
}

