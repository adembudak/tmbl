#include "cpu.h"

namespace tmbl::cpu {

void cpu::LD(reg8 &r, reg16 &rr, u8 CY) noexcept {
  r = m[rr];
  c.tick(CY);
}

void cpu::LD(reg16 &rr, reg8 &r, u8 CY) noexcept {
  m[rr] = byte(r.data());
  c.tick(CY);
}

void cpu::LD(reg16 &rr, u8 n, u8 CY) noexcept {
  m[rr] = byte(n);
  c.tick(CY);
}

void cpu::LD(reg8 &r1, reg8 &r2, u8 CY, [[maybe_unused]] int dummy) noexcept {
  m[r1] = byte(r2.data());
  c.tick(CY);
}

void cpu::LD(reg8 &r1, reg8 &r2, u8 CY) noexcept {
  r1 = r2;
  c.tick(CY);
}

void cpu::LD(reg8 &r, u8 n, u8 CY, [[maybe_unused]] int dummy) noexcept {
  r = m[n];
  c.tick(CY);
}

void cpu::LD(reg8 &r, u8 n, u8 CY) noexcept {
  r = n;
  c.tick(CY);
}

void cpu::LD(u8 n, reg8 r, u8 CY) noexcept {
  m[n] = byte(r.data());
  c.tick(CY);
}

void cpu::LD(reg8 &r, u16 nn, u8 CY) noexcept {
  r = m[nn];
  c.tick(CY);
}

void cpu::LD(u16 nn, reg8 &r, u8 CY) noexcept {
  m[nn] = byte(r.data());
  c.tick(CY);
}

void cpu::LD(u16 nn, reg16 &rr, u8 CY) noexcept {
  m[nn] = byte(rr.data());
  c.tick(CY);
}

void cpu::LD(reg16 &rr1, reg16 &rr2, u8 CY) noexcept {
  rr1 = rr2;
  c.tick(CY);
}

void cpu::LD(reg16 &rr1, u16 nn, u8 CY) noexcept {
  rr1 = nn;
  c.tick(CY);
}

void cpu::PUSH(const reg16 rr) {
  m[SP - 1] = rr.lo();
  m[SP - 2] = rr.hi();
  SP -= 2;

  c.tick(4);
}

void cpu::POP(reg16 rr) {
  rr.lo(m[SP]);
  rr.hi(m[SP + 1]);
  SP += 2;

  c.tick(3);
}
void cpu::ADD(const reg8 r) {
  ((r.data() + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((r.loNibble() + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (r.data() + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  A = A + r;

  c.tick(1);
}

void cpu::ADD(const u8 n) {
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A + tmp;

  c.tick(2);
}

void cpu::ADD(const reg16 rr) {
  u8 n = std::to_integer<unsigned>(m[rr]);
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];
  A = A + tmp;

  c.tick(2);
}

void cpu::ADC(const reg8 r) {

  if (A.C()) {
    A += 1;
  }

  ((r.data() + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((r.loNibble() + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (r.data() + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  A = A + r;
  c.tick(1);
}

void cpu::ADC(const u8 n) {
  if (F.C()) {
    A += 1;
  }

  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A + tmp;
  c.tick(2);
}

void cpu::ADC(const reg16 rr) {
  if (F.C()) {
    A += 1;
  }

  u8 n = std::to_integer<unsigned>(m[rr]);
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A + tmp;
  c.tick(2);
}

void cpu::SUB(const reg8 r) {
  ((A.data() - r.data()) == reg8::min()) ? F.Z(set) : F.Z(reset);
  ((A.loNibble() < r.loNibble())) ? F.H(set) : F.H(reset);
  F.N(set);
  (A.data() < r.data()) ? F.C(set) : F.C(reset);

  A = A - r;
  c.tick(1);
}

void cpu::SUB(const u8 n) {

  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  F.N(set);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A - tmp;
  c.tick(2);
}

void cpu::SUB(const reg16 rr) {

  u8 n = std::to_integer<unsigned>(m[rr]);
  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  F.N(set);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A - tmp;
  c.tick(2);
}

void cpu::SBC(const reg8 r) {
  if (F.C()) {
    A -= 1;
  }
  ((A.data() - r.data()) == reg8::min()) ? F.Z(set) : F.Z(reset);
  ((A.loNibble() < r.loNibble())) ? F.H(set) : F.H(reset);
  F.N(set);
  (A.data() < r.data()) ? F.C(set) : F.C(reset);

  A = A - r;
  c.tick(1);
}

void cpu::SBC(const u8 n) {
  if (F.C()) {
    A -= 1;
  }

  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  F.N(set);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A - tmp;
  c.tick(2);
}

void cpu::SBC(const reg16 rr) {
  if (F.C()) {
    A -= 1;
  }

  u8 n = std::to_integer<unsigned>(m[rr]);
  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  F.N(set);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A - tmp;
  c.tick(2);
}

void cpu::AND(const reg8 r) {
  F.C(reset);
  F.H(set);
  F.N(reset);
  (A.data() == 0 || r.data() == 0) ? F.Z(set) : F.Z(reset);

  A = A & r;
  c.tick(1);
}
void cpu::AND(const u8 n) {
  F.C(reset);
  F.H(set);
  F.N(reset);

  (A.data() == 0 || n == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A & tmp;
  c.tick(2);
}

void cpu::AND(const reg16 rr) {
  F.C(reset);
  F.H(set);
  F.N(reset);
  (A.data() == 0 || std::to_integer<unsigned>(m[rr]) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A & tmp;
  c.tick(2);
}

void cpu::OR(const reg8 r) {
  F.C(reset);
  F.H(reset);
  F.N(reset);
  (A.data() == 0 && r.data() == 0) ? F.Z(set) : F.Z(reset);

  A = A | r;
  c.tick(1);
}
void cpu::OR(const u8 n) {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.data() == 0 || n == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A | tmp;
  c.tick(2);
}

void cpu::OR(const reg16 rr) {
  F.C(reset);
  F.H(reset);
  F.N(reset);
  (A.data() == 0 || std::to_integer<unsigned>(m[rr]) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A | tmp;
  c.tick(2);
}

void cpu::XOR(const reg8 r) {
  F.C(reset);
  F.H(reset);
  F.N(reset);
  (A.data() == r.data()) ? F.Z(set) : F.Z(reset);

  A = A ^ r;
  c.tick(1);
}

void cpu::XOR(const u8 n) {
  F.C(reset);
  F.H(reset);
  F.N(reset);
  (A.data() == n) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A ^ tmp;
  c.tick(2);
}

void cpu::XOR(const reg16 rr) {
  F.C(reset);
  F.H(reset);
  F.N(reset);
  (A.data() == std::to_integer<unsigned>(m[rr])) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A ^ tmp;
  c.tick(2);
}

void cpu::CP(const reg8 r) {
  (r.data() > A.data()) ? F.C(set) : F.C(reset);
  (r.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  F.N(set);
  (r.data() == A.data()) ? F.Z(set) : F.Z(reset);

  c.tick(1);
}

void cpu::CP(const u8 n) {
  reg8 tmp;
  tmp = n;

  (tmp.data() > A.data()) ? F.C(set) : F.C(reset);
  (tmp.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  F.N(set);
  (tmp.data() == A.data()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::CP(const reg16 rr) {
  reg8 tmp;
  tmp = m[rr];

  (tmp.data() > A.data()) ? F.C(set) : F.C(reset);
  (tmp.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  F.N(set);
  (tmp.data() == A.data()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::INC(reg8 r) {
  (r.loNibble() == 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (r.data() == 0b1111'1111) ? F.Z(set) : F.Z(reset);

  r += 1;
  c.tick(1);
}

void cpu::INC(reg16 rr) {
  u8 lower_nibble = std::to_integer<unsigned>(m[rr]) | (0b0000'1111U);

  (lower_nibble == 0b0000'1111) ? F.H(set) : F.H(reset);
  F.N(reset);
  (std::to_integer<unsigned>(m[rr]) == 0b1111'1111) ? F.Z(set) : F.Z(reset);

  m[rr] = byte(std::to_integer<unsigned>(m[rr]) + 1);
  c.tick(3);
}

void cpu::DEC(reg8 r) {
  (r.loNibble() == 0) ? F.H(set) : F.H(reset);
  F.N(set);
  (r.data() == 1) ? F.Z(set) : F.Z(reset);

  r -= 1;
  c.tick(1);
}

void cpu::DEC(reg16 rr) {
  u8 lower_nibble = std::to_integer<unsigned>(m[rr]) | (0b0000'1111U);

  (lower_nibble == 0) ? F.H(set) : F.H(reset);
  F.N(set);
  (std::to_integer<unsigned>(m[rr]) == 1) ? F.Z(set) : F.Z(reset);

  m[rr] = byte(std::to_integer<unsigned>(m[rr]) - 1);
  c.tick(3);
}

void cpu::ADD(reg16 rr1, reg16 rr2) {
  (rr1.data() + rr2.data() > reg16::max()) ? F.C(set) : F.C(reset);

  // zero the 4th nibble of both register to check H flag state;
  // if the sume of first 3rd nibble values bigger than 4096, then H flag set
  // TODO: write a helper function check H value state;
  ((rr1.data() & 0b0000'1111'1111'1111) + (rr2.data() & 0b0000'1111'1111'1111) > 4095U)
      ? F.H(set)
      : F.H(reset);

  F.N(reset);

  rr1 = rr1 + rr2;
  c.tick(2);
}

void cpu::ADD(const u8 n, [[maybe_unused]] int dummy) {
  ((SP.data() + n) > reg16::max()) ? F.C(set) : F.C(reset);
  (((SP.data() & 0b0000'1111'1111'1111) + n) > 4095U) ? F.H(set) : F.H(reset);

  F.N(reset);
  F.Z(reset);

  SP += n;
  c.tick(4);
}

void cpu::INC(reg16 rr, [[maybe_unused]] int dummy) {
  rr += 1;
  c.tick(2);
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

