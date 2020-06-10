#include "cpu.h"
#include <bit>

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

void cpu::PUSH(const reg16 rr) noexcept {
  m[SP - 1] = rr.lo();
  m[SP - 2] = rr.hi();
  SP -= 2;

  c.tick(4);
}

void cpu::POP(reg16 rr) noexcept {
  rr.lo(m[SP]);
  rr.hi(m[SP + 1]);
  SP += 2;

  c.tick(3);
}
void cpu::ADD(const reg8 r) noexcept {
  F.N(reset);
  ((r.data() + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((r.loNibble() + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (r.data() + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  A = A + r;

  c.tick(1);
}

void cpu::ADD(const u8 n) noexcept {
  F.N(reset);
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A + tmp;

  c.tick(2);
}

void cpu::ADD(const reg16 rr) noexcept {
  F.N(reset);
  u8 n = std::to_integer<unsigned>(m[rr]);
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];
  A = A + tmp;

  c.tick(2);
}

void cpu::ADC(const reg8 r) noexcept {
  F.N(reset);

  if (A.C()) {
    A += 1;
  }

  ((r.data() + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((r.loNibble() + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (r.data() + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  A = A + r;
  c.tick(1);
}

void cpu::ADC(const u8 n) noexcept {
  F.N(reset);

  if (F.C()) {
    A += 1;
  }

  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A + tmp;
  c.tick(2);
}

void cpu::ADC(const reg16 rr) noexcept {
  F.N(reset);

  if (F.C()) {
    A += 1;
  }

  u8 n = std::to_integer<unsigned>(m[rr]);
  ((n + A.data()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.data() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A + tmp;
  c.tick(2);
}

void cpu::SUB(const reg8 r) noexcept {
  F.N(set);

  ((A.data() - r.data()) == reg8::min()) ? F.Z(set) : F.Z(reset);
  ((A.loNibble() < r.loNibble())) ? F.H(set) : F.H(reset);
  (A.data() < r.data()) ? F.C(set) : F.C(reset);

  A = A - r;
  c.tick(1);
}

void cpu::SUB(const u8 n) noexcept {
  F.N(set);

  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A - tmp;
  c.tick(2);
}

void cpu::SUB(const reg16 rr) noexcept {
  F.N(set);

  u8 n = std::to_integer<unsigned>(m[rr]);
  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A - tmp;
  c.tick(2);
}

void cpu::SBC(const reg8 r) noexcept {
  F.N(set);

  if (F.C()) {
    A -= 1;
  }
  ((A.data() - r.data()) == reg8::min()) ? F.Z(set) : F.Z(reset);
  ((A.loNibble() < r.loNibble())) ? F.H(set) : F.H(reset);
  (A.data() < r.data()) ? F.C(set) : F.C(reset);

  A = A - r;
  c.tick(1);
}

void cpu::SBC(const u8 n) noexcept {
  F.N(set);

  if (F.C()) {
    A -= 1;
  }

  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A - tmp;
  c.tick(2);
}

void cpu::SBC(const reg16 rr) noexcept {
  F.N(set);

  if (F.C()) {
    A -= 1;
  }

  u8 n = std::to_integer<unsigned>(m[rr]);
  ((A.data() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.data() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A - tmp;
  c.tick(2);
}

void cpu::AND(const reg8 r) noexcept {
  F.C(reset);
  F.H(set);
  F.N(reset);

  (A.data() == 0 || r.data() == 0) ? F.Z(set) : F.Z(reset);

  A = A & r;
  c.tick(1);
}
void cpu::AND(const u8 n) noexcept {
  F.C(reset);
  F.H(set);
  F.N(reset);

  (A.data() == 0 || n == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A & tmp;
  c.tick(2);
}

void cpu::AND(const reg16 rr) noexcept {
  F.C(reset);
  F.H(set);
  F.N(reset);

  (A.data() == 0 || std::to_integer<unsigned>(m[rr]) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A & tmp;
  c.tick(2);
}

void cpu::OR(const reg8 r) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.data() == 0 && r.data() == 0) ? F.Z(set) : F.Z(reset);

  A = A | r;
  c.tick(1);
}
void cpu::OR(const u8 n) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.data() == 0 || n == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A | tmp;
  c.tick(2);
}

void cpu::OR(const reg16 rr) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.data() == 0 || std::to_integer<unsigned>(m[rr]) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A | tmp;
  c.tick(2);
}

void cpu::XOR(const reg8 r) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.data() == r.data()) ? F.Z(set) : F.Z(reset);

  A = A ^ r;
  c.tick(1);
}

void cpu::XOR(const u8 n) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.data() == n) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A ^ tmp;
  c.tick(2);
}

void cpu::XOR(const reg16 rr) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.data() == std::to_integer<unsigned>(m[rr])) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A ^ tmp;
  c.tick(2);
}

void cpu::CP(const reg8 r) noexcept {
  F.N(set);

  (r.data() > A.data()) ? F.C(set) : F.C(reset);
  (r.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  (r.data() == A.data()) ? F.Z(set) : F.Z(reset);

  c.tick(1);
}

void cpu::CP(const u8 n) noexcept {
  F.N(set);

  reg8 tmp;
  tmp = n;

  (tmp.data() > A.data()) ? F.C(set) : F.C(reset);
  (tmp.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  (tmp.data() == A.data()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::CP(const reg16 rr) noexcept {
  F.N(set);

  reg8 tmp;
  tmp = m[rr];

  (tmp.data() > A.data()) ? F.C(set) : F.C(reset);
  (tmp.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  (tmp.data() == A.data()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::INC(reg8 r) noexcept {
  F.N(reset);

  (r.loNibble() == 0b0000'1111) ? F.H(set) : F.H(reset);
  (r.data() == 0b1111'1111) ? F.Z(set) : F.Z(reset);

  r += 1;
  c.tick(1);
}

void cpu::INC(reg16 rr) noexcept {
  F.N(reset);

  u8 lower_nibble = std::to_integer<unsigned>(m[rr]) | (0b0000'1111U);

  (lower_nibble == 0b0000'1111) ? F.H(set) : F.H(reset);
  (std::to_integer<unsigned>(m[rr]) == 0b1111'1111) ? F.Z(set) : F.Z(reset);

  m[rr] = byte(std::to_integer<unsigned>(m[rr]) + 1);
  c.tick(3);
}

void cpu::DEC(reg8 r) noexcept {
  F.N(set);

  (r.loNibble() == 0) ? F.H(set) : F.H(reset);
  (r.data() == 1) ? F.Z(set) : F.Z(reset);

  r -= 1;
  c.tick(1);
}

void cpu::DEC(reg16 rr) noexcept {
  F.N(set);

  u8 lower_nibble = std::to_integer<unsigned>(m[rr]) | (0b0000'1111U);

  (lower_nibble == 0) ? F.H(set) : F.H(reset);
  (std::to_integer<unsigned>(m[rr]) == 1) ? F.Z(set) : F.Z(reset);

  m[rr] = byte(std::to_integer<unsigned>(m[rr]) - 1);
  c.tick(3);
}

void cpu::ADD(reg16 rr1, reg16 rr2) noexcept {
  F.N(reset);

  (rr1.data() + rr2.data() > reg16::max()) ? F.C(set) : F.C(reset);

  // zero the 4th nibble of both register to check H flag state;
  // if the sume of first 3rd nibble values bigger than 4096, then H flag set
  // TODO: write a helper function check H value state;
  ((rr1.data() & 0b0000'1111'1111'1111U) + (rr2.data() & 0b0000'1111'1111'1111U) > 4095U)
      ? F.H(set)
      : F.H(reset);

  rr1 = rr1 + rr2;
  c.tick(2);
}

void cpu::ADD(const u8 n, [[maybe_unused]] int dummy) noexcept {
  F.N(reset);
  F.Z(reset);

  ((SP.data() + n) > reg16::max()) ? F.C(set) : F.C(reset);
  (((SP.data() & 0b0000'1111'1111'1111U) + n) > 4095U) ? F.H(set) : F.H(reset);

  SP += n;
  c.tick(4);
}

void cpu::INC(reg16 rr, [[maybe_unused]] int dummy) noexcept {
  rr += 1;
  c.tick(2);
}

void cpu::DEC(reg16 rr, [[maybe_unused]] int dummy) noexcept {
  rr -= 1;
  c.tick(2);
}

void cpu::RLCA() noexcept {
  F.H(reset);
  F.N(reset);
  F.Z(reset);

  // rotate left
  A = std::rotl(A.data(), /*rotate 1 times=*/1);
  (A.msb() == 1) ? F.C(set) : F.C(reset);

  c.tick(1);
}

void cpu::RLA() noexcept {
  F.H(reset);
  F.N(reset);
  F.Z(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;

  A = std::rotl(A.data(), 1);
  (A.msb() == 1) ? F.C(set) : F.C(reset);

  A = A.data() | mask_for_0th_bit;
  c.tick(1);
}

void cpu::RRCA() noexcept {
  F.H(reset);
  F.N(reset);
  F.Z(reset);

  A = std::rotr(A.data(), 1);
  (A.lsb() == 1) ? F.C(set) : F.C(reset);

  c.tick(1);
}

void cpu::RRA() noexcept {
  F.H(reset);
  F.N(reset);
  F.Z(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  A = std::rotr(A.data(), 1);
  (A.lsb() == 1) ? F.C(set) : F.C(reset);

  A = A.data() | mask_for_7th_bit;
  c.tick(1);
}

void cpu::RLC(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  r = std::rotl(r.data(), 1);
  (A.msb() == 1) ? F.C(set) : F.C(reset);
  (A.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::RLC(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  m[rr] = byte(std::rotl(std::to_integer<unsigned>(m[rr]), 1));

  reg8 tmp;
  tmp = m[rr];

  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::RL(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;

  r = std::rotl(r.data(), 1);

  (r.msb() == 1) ? F.C(set) : F.C(reset);
  (r.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  r = r.data() | mask_for_0th_bit;

  c.tick(2);
}

void cpu::RL(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;
  m[rr] = byte(std::rotl(std::to_integer<unsigned>(m[rr]), 1));

  reg8 tmp;
  tmp = m[rr];

  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  m[rr] = m[rr] | byte(mask_for_0th_bit);

  c.tick(4);
}

void cpu::RRC(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  r = std::rotr(r.data(), 1);
  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::RRC(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  m[rr] = byte(std::rotr(std::to_integer<unsigned>(m[rr]), 1));

  // create a temporary reg8 to reach individual bits of the byte
  // more easily, and set flags via it.
  reg8 tmp;
  tmp = m[rr];
  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.data() == 0) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::RR(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  r = std::rotr(r.data(), 1);

  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  r = r.data() | mask_for_7th_bit;

  c.tick(2);
}

void cpu::RR(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  m[rr] = byte(std::rotr(std::to_integer<unsigned>(m[rr]), 1));

  reg8 tmp;
  tmp = m[rr];

  (tmp.lsb() == 1) ? F.C(set) : F.C(reset);
  (tmp.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  m[rr] = m[rr] | byte(mask_for_7th_bit);

  c.tick(4);
}

void cpu::SLA(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  (r.msb() == 1) ? F.C(set) : F.C(reset);
  r = r.data() << 1U;
  (r.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SLA(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  reg8 tmp;
  tmp = m[rr];
  (tmp.msb() == 1) ? F.C(set) : F.C(reset);

  m[rr] <<= 1;
  (std::to_integer<unsigned>(m[rr]) == 0U) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::SRA(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 old_msb_mask = (r.msb() == 1) ? 0b1000'0000 : 0b0000'0000;

  r = r.data() >> 1U;
  r = r.data() | old_msb_mask;
  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.data() == 0) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SRA(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  reg8 tmp;
  tmp = m[rr];
  u8 old_msb_mask = (tmp.msb() == 1) ? 0b1000'0000 : 0b0000'0000;
  (tmp.lsb() == 1) ? F.C(set) : F.C(reset);

  m[rr] >>= 1;
  m[rr] |= byte(old_msb_mask);
  (std::to_integer<unsigned>(m[rr]) == 0U) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::SRL(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  r = r.data() >> 1;
  (r.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SRL(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  reg8 tmp;
  tmp = m[rr];
  (tmp.lsb() == 1) ? F.C(set) : F.C(reset);

  m[rr] >>= 1;
  (std::to_integer<unsigned>(m[rr]) == 0U) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::SWAP(reg8 &r) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  r = std::rotl(r.data(), 4);

  (r.data() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SWAP(reg16 &rr) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  m[rr] = byte(std::rotl(std::to_integer<unsigned>(m[rr]), 4));

  (std::to_integer<unsigned>(m[rr]) == 0U) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::BIT(reg8 r, u8 pos) {
  F.H(set);
  F.N(reset);

  r.test(pos) ? F.Z(reset) : F.Z(set);

  c.tick(2);
}

void cpu::BIT(reg16 rr, u8 pos) {
  F.H(set);
  F.N(reset);

  reg8 tmp;
  tmp = m[rr];

  tmp.test(pos) ? F.Z(reset) : F.Z(set);

  c.tick(3);
}

void cpu::SET(reg8 &r, u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  // TODO: package this binary hack to
  // reg8 class, overload operator[] ?
  r = r.data() | (1U << pos);

  c.tick(2);
}

void cpu::SET(reg16 rr, u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  m[rr] = m[rr] | byte((1U << pos));

  c.tick(4);
}

void cpu::RES(reg8 &r, u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  r = r.data() & ~(1U << pos);

  c.tick(2);
}

void cpu::RES(reg16 rr, u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  m[rr] = m[rr] & ~byte((1U << pos));

  c.tick(4);
}

void cpu::JP(u16 nn) noexcept {
  PC = nn;

  c.tick(4);
}

void cpu::JP(u8 cc, u16 nn) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    PC = nn;
    c.tick(4);
  } else {
    c.tick(3);
  }
}

void cpu::JP(reg16 rr) noexcept {
  PC = rr;

  c.tick(1);
}

void cpu::JR(i8 e) noexcept {
  PC += e;

  c.tick(3);
}

void cpu::JR(u8 cc, i8 e) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    PC += e;
    c.tick(3);
  } else {
    c.tick(2);
  }
}

void cpu::CALL(u16 nn) noexcept {
  m[SP - 1] = PC.hi();
  m[SP - 2] = PC.lo();
  PC = nn;
  SP = SP - 2;

  c.tick(6);
}

void cpu::CALL(u8 cc, u16 nn) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    m[SP - 1] = PC.hi();
    m[SP - 2] = PC.lo();
    PC = nn;
    SP = SP - 2;

    c.tick(6);
  } else {
    c.tick(3);
  }
}

void cpu::RET() noexcept {
  PC.lo(m[SP]);
  PC.hi(m[SP + 1]);
  SP = SP + 2;

  c.tick(4);
}

void cpu::RETI() noexcept {
  PC.lo(m[SP]);
  PC.hi(m[SP + 1]);
  SP = SP + 2;

  IME = set;

  c.tick(4);
}

void cpu::RET(u8 cc) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    PC.lo(m[SP]);
    PC.hi(m[SP + 1]);
    SP = SP + 2;

    c.tick(5);
  } else {
    c.tick(2);
  }
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

