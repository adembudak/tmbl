#include "cpu.h"
#include <stdexcept>

namespace tmbl::cpu {

void cpu::run() {
  // clang-format off
  auto make_i8 = [](byte b) -> i8 { return std::to_integer<i8>(b); };
  auto make_u8 = [](byte b) -> u8 { return std::to_integer<unsigned>(b); };
  auto make_u16 = [](byte b) -> u16 { return std::to_integer<unsigned>(b); };
  // clang-format on

  u16 nn = 0;
  u8 n = 0;
  i8 e = 0;

  reg8 D, E;
  reg8 B, C;
  reg8 H, L;

  for (;;) {
    // clang-format off
    D = DE.hi(); E = DE.lo();
    B = BC.hi(); C = BC.lo();
    H = HL.hi(); L = HL.lo();
    // clang-format on

    byte addr = fetch(PC++);

    switch (u16 val = decode(addr); val) {
      case 0x00:
        NOP();
        break;

      case 0x01:
        nn = make_u16(fetch(PC++));
        LD(BC, nn);
        break;

      case 0x02:
        LD(BC, A);
        break;

      case 0x03:
        INC(BC);
        break;

      case 0x04:
        INC(B);
        BC.hi(B);
        break;

      case 0x05:
        DEC(B);
        BC.hi(B);
        break;

      case 0x06:
        n = make_u8(fetch(PC++));
        LD(B, n);
        break;

      case 0x07:
        RLCA();
        break;

      case 0x08:
        nn = make_u16(fetch(PC++));
        LD(nn, SP);
        break;

      case 0x09:
        ADD(HL, BC);
        break;

      case 0x0A:
        LD(A, BC);
        break;

      case 0x0B:
        DEC(BC, /*dummy*/ 666);
        break;

      case 0x0C:
        INC(C);
        BC.lo(C);
        break;

      case 0x0D:
        DEC(C);
        break;

      case 0x0E:
        n = make_u8(fetch(PC++));
        LD(C, n);
        BC.lo(C);
        break;

      case 0x0F:
        RRCA();
        break;

      case 0x10:
        STOP();
        break;

      case 0x11:
        nn = make_u16(fetch(PC++));
        LD(DE, nn);
        break;

      case 0x12:
        LD(DE, A);
        break;

      case 0x13:
        INC(DE);
        break;

      case 0x14:
        INC(D);
        DE.hi(D);
        break;

      case 0x15:
        DEC(D);
        DE.hi(D);
        break;

      case 0x16:
        n = make_u8(fetch(PC++));
        LD(D, n);
        DE.hi(D);
        break;

      case 0x17:
        RLA();
        break;

      case 0x18:
        e = make_i8(fetch(PC++));
        JR(e);
        break;

      case 0x19:
        ADD(HL, DE);
        break;

      case 0x1A:
        LD(A, DE);
        break;

      case 0x1B:
        DEC(DE, /*dummy*/ 666);
        break;

      case 0x1C:
        INC(E);
        DE.lo(E);
        break;

      case 0x1D:
        DEC(E);
        DE.lo(E);
        break;

      case 0x1E:
        n = make_u8(fetch(PC++));
        LD(E, n);
        DE.lo(E);
        break;

      case 0x1F:
        RRA();

      default:;
    }
  }
}

void cpu::LD(reg8 &r1, const reg8 r2) noexcept {
  r1 = r2;

  c.tick(1);
}

void cpu::LD(reg8 &r, const u8 n) noexcept {
  r = n;

  c.tick(2);
}

void cpu::LD(reg8 &r, const reg16 rr) noexcept {
  r = m[rr];

  c.tick(2);
}

void cpu::LD(const reg16 rr, const reg8 r) noexcept {
  m[rr] = byte(r.value());

  c.tick(2);
}

void cpu::LDi() noexcept {
  m[HL] = byte(A.value());
  HL += 1;

  c.tick(2);
}

void cpu::LDi([[maybe_unused]] int dummy) noexcept {
  A = m[HL];
  HL += 1;

  c.tick(2);
}

void cpu::LDd() noexcept {
  m[HL] = byte(A.value());
  HL -= 1;

  c.tick(2);
}

void cpu::LDd([[maybe_unused]] int dummy) noexcept {
  A = m[HL];
  HL -= 1;

  c.tick(2);
}

void cpu::LD(const u16 nn) noexcept {
  m[nn] = byte(A.value());

  c.tick(4);
}

void cpu::LD(const u16 nn, [[maybe_unused]] int dummy) noexcept {
  A = m[nn];

  c.tick(4);
}

void cpu::LD(const u8 n, Orientation o) noexcept {
  switch (o) {
    case Orientation::READ_FROM_IO_PORT:
      A = m[u16(0xFF00 + n)];
      break;
    case Orientation::WRITE_TO_IO_PORT:
      m[u16(0xFF00 + n)] = byte(A.value());
      break;
  }

  c.tick(3);
}

void cpu::LD(Orientation o) noexcept {

  u16 C_reg_val = std::to_integer<unsigned>(BC.lo());

  switch (o) {
    case Orientation::READ_FROM_IO_PORT:
      A = m[u16(0xFF00 + C_reg_val)];
      break;
    case Orientation::WRITE_TO_IO_PORT:
      m[u16(0xFF00 + C_reg_val)] = byte(A.value());
      break;
  }

  c.tick(2);
}

void cpu::LD(const reg16 rr, const u8 n, [[maybe_unused]] int dummy) noexcept {
  m[rr] = byte(n);

  c.tick(3);
}

void cpu::LD(reg16 &rr, const u16 nn) noexcept {
  rr.lo(byte(nn & 0x00FFU));
  rr.hi(byte((nn & 0xFF00U) >> 8));

  c.tick(3);
}

void cpu::LD() noexcept {
  SP = HL;

  c.tick(2);
}

void cpu::LD(const u8 i) noexcept {
  HL = SP + i;

  c.tick(3);
}

void cpu::LD(const u16 nn, const reg16 rr) noexcept {
  m[nn] = rr.lo();
  m[nn + 1] = rr.hi();

  c.tick(5);
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
  ((r.value() + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((r.loNibble() + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (r.value() + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

  A = A + r;

  c.tick(1);
}

void cpu::ADD(const u8 n) noexcept {
  F.N(reset);
  ((n + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A + tmp;

  c.tick(2);
}

void cpu::ADD(const reg16 rr) noexcept {
  F.N(reset);
  u8 n = std::to_integer<unsigned>(m[rr]);
  ((n + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

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

  ((r.value() + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((r.loNibble() + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (r.value() + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

  A = A + r;
  c.tick(1);
}

void cpu::ADC(const u8 n) noexcept {
  F.N(reset);

  if (F.C()) {
    A += 1;
  }

  ((n + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

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
  ((n + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A + tmp;
  c.tick(2);
}

void cpu::SUB(const reg8 r) noexcept {
  F.N(set);

  ((A.value() - r.value()) == reg8::min()) ? F.Z(set) : F.Z(reset);
  ((A.loNibble() < r.loNibble())) ? F.H(set) : F.H(reset);
  (A.value() < r.value()) ? F.C(set) : F.C(reset);

  A = A - r;
  c.tick(1);
}

void cpu::SUB(const u8 n) noexcept {
  F.N(set);

  ((A.value() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.value() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = n;

  A = A - tmp;
  c.tick(2);
}

void cpu::SUB(const reg16 rr) noexcept {
  F.N(set);

  u8 n = std::to_integer<unsigned>(m[rr]);
  ((A.value() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.value() < n) ? F.C(set) : F.C(reset);

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
  ((A.value() - r.value()) == reg8::min()) ? F.Z(set) : F.Z(reset);
  ((A.loNibble() < r.loNibble())) ? F.H(set) : F.H(reset);
  (A.value() < r.value()) ? F.C(set) : F.C(reset);

  A = A - r;
  c.tick(1);
}

void cpu::SBC(const u8 n) noexcept {
  F.N(set);

  if (F.C()) {
    A -= 1;
  }

  ((A.value() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.value() < n) ? F.C(set) : F.C(reset);

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
  ((A.value() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.value() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A - tmp;
  c.tick(2);
}

void cpu::AND(const reg8 r) noexcept {
  F.C(reset);
  F.H(set);
  F.N(reset);

  (A.value() == 0 || r.value() == 0) ? F.Z(set) : F.Z(reset);

  A = A & r;
  c.tick(1);
}
void cpu::AND(const u8 n) noexcept {
  F.C(reset);
  F.H(set);
  F.N(reset);

  (A.value() == 0 || n == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A & tmp;
  c.tick(2);
}

void cpu::AND(const reg16 rr) noexcept {
  F.C(reset);
  F.H(set);
  F.N(reset);

  (A.value() == 0 || std::to_integer<unsigned>(m[rr]) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A & tmp;
  c.tick(2);
}

void cpu::OR(const reg8 r) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.value() == 0 && r.value() == 0) ? F.Z(set) : F.Z(reset);

  A = A | r;
  c.tick(1);
}
void cpu::OR(const u8 n) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.value() == 0 || n == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A | tmp;
  c.tick(2);
}

void cpu::OR(const reg16 rr) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.value() == 0 || std::to_integer<unsigned>(m[rr]) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A | tmp;
  c.tick(2);
}

void cpu::XOR(const reg8 r) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.value() == r.value()) ? F.Z(set) : F.Z(reset);

  A = A ^ r;
  c.tick(1);
}

void cpu::XOR(const u8 n) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.value() == n) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = n;

  A = A ^ tmp;
  c.tick(2);
}

void cpu::XOR(const reg16 rr) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  (A.value() == std::to_integer<unsigned>(m[rr])) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = m[rr];

  A = A ^ tmp;
  c.tick(2);
}

void cpu::CP(const reg8 r) noexcept {
  F.N(set);

  (r.value() > A.value()) ? F.C(set) : F.C(reset);
  (r.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  (r.value() == A.value()) ? F.Z(set) : F.Z(reset);

  c.tick(1);
}

void cpu::CP(const u8 n) noexcept {
  F.N(set);

  reg8 tmp;
  tmp = n;

  (tmp.value() > A.value()) ? F.C(set) : F.C(reset);
  (tmp.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  (tmp.value() == A.value()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::CP(const reg16 rr) noexcept {
  F.N(set);

  reg8 tmp;
  tmp = m[rr];

  (tmp.value() > A.value()) ? F.C(set) : F.C(reset);
  (tmp.loNibble() > A.loNibble()) ? F.H(set) : F.H(reset);
  (tmp.value() == A.value()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::INC(reg8 &r) noexcept {
  F.N(reset);

  (r.loNibble() == 0b0000'1111) ? F.H(set) : F.H(reset);
  (r.value() == 0b1111'1111) ? F.Z(set) : F.Z(reset);

  r += 1;
  c.tick(1);
}

void cpu::INC() noexcept {
  F.N(reset);

  u8 lower_nibble = std::to_integer<unsigned>(m[HL]) | (0b0000'1111U);

  (lower_nibble == 0b0000'1111) ? F.H(set) : F.H(reset);
  (std::to_integer<unsigned>(m[HL]) == 0b1111'1111) ? F.Z(set) : F.Z(reset);

  m[HL] = byte(std::to_integer<unsigned>(m[HL]) + 1);
  c.tick(3);
}

void cpu::INC(reg16 &rr) noexcept {
  rr = rr + 1;
  c.tick(2);
}

void cpu::DEC(reg8 &r) noexcept {
  F.N(set);

  (r.loNibble() == 0) ? F.H(set) : F.H(reset);
  (r.value() == 1) ? F.Z(set) : F.Z(reset);

  r -= 1;
  c.tick(1);
}

void cpu::DEC(const reg16 rr) noexcept {
  F.N(set);

  u8 lower_nibble = std::to_integer<unsigned>(m[rr]) | (0b0000'1111U);

  (lower_nibble == 0) ? F.H(set) : F.H(reset);
  (std::to_integer<unsigned>(m[rr]) == 1) ? F.Z(set) : F.Z(reset);

  m[rr] = byte(std::to_integer<unsigned>(m[rr]) - 1);
  c.tick(3);
}

void cpu::ADD(reg16 &rr1, const reg16 rr2) noexcept {
  F.N(reset);

  (rr1.value() + rr2.value() > reg16::max()) ? F.C(set) : F.C(reset);

  // zero the 4th nibble of both register to check H flag state;
  // if the sume of first 3rd nibble values bigger than 4096, then H flag set
  // TODO: write a helper function check H value state;
  ((rr1.value() & 0b0000'1111'1111'1111U) + (rr2.value() & 0b0000'1111'1111'1111U) > 4095U)
      ? F.H(set)
      : F.H(reset);

  rr1 = rr1 + rr2;
  c.tick(2);
}

void cpu::ADD(const u8 n, [[maybe_unused]] int dummy) noexcept {
  F.N(reset);
  F.Z(reset);

  ((SP.value() + n) > reg16::max()) ? F.C(set) : F.C(reset);
  (((SP.value() & 0b0000'1111'1111'1111U) + n) > 4095U) ? F.H(set) : F.H(reset);

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
  A = rotl(A.value(), /*rotate 1 times=*/1);
  (A.msb() == 1) ? F.C(set) : F.C(reset);

  c.tick(1);
}

void cpu::RLA() noexcept {
  F.H(reset);
  F.N(reset);
  F.Z(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;

  A = rotl(A.value(), 1);
  (A.msb() == 1) ? F.C(set) : F.C(reset);

  A = A.value() | mask_for_0th_bit;
  c.tick(1);
}

void cpu::RRCA() noexcept {
  F.H(reset);
  F.N(reset);
  F.Z(reset);

  A = rotr(A.value(), 1);
  (A.lsb() == 1) ? F.C(set) : F.C(reset);

  c.tick(1);
}

void cpu::RRA() noexcept {
  F.H(reset);
  F.N(reset);
  F.Z(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  A = rotr(A.value(), 1);
  (A.lsb() == 1) ? F.C(set) : F.C(reset);

  A = A.value() | mask_for_7th_bit;
  c.tick(1);
}

void cpu::RLC(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  r = rotl(r.value(), 1);
  (A.msb() == 1) ? F.C(set) : F.C(reset);
  (A.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::RLC(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  m[rr] = byte(rotl(std::to_integer<unsigned>(m[rr]), 1));

  reg8 tmp;
  tmp = m[rr];

  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::RL(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;

  r = rotl(r.value(), 1);

  (r.msb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  r = r.value() | mask_for_0th_bit;

  c.tick(2);
}

void cpu::RL(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;
  m[rr] = byte(rotl(std::to_integer<unsigned>(m[rr]), 1));

  reg8 tmp;
  tmp = m[rr];

  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  m[rr] = m[rr] | byte(mask_for_0th_bit);

  c.tick(4);
}

void cpu::RRC(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  r = rotr(r.value(), 1);
  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::RRC(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  m[rr] = byte(rotr(std::to_integer<unsigned>(m[rr]), 1));

  // create a temporary reg8 to reach individual bits of the byte
  // more easily, and set flags via it.
  reg8 tmp;
  tmp = m[rr];
  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == 0) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::RR(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  r = rotr(r.value(), 1);

  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  r = r.value() | mask_for_7th_bit;

  c.tick(2);
}

void cpu::RR(reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  m[rr] = byte(rotr(std::to_integer<unsigned>(m[rr]), 1));

  reg8 tmp;
  tmp = m[rr];

  (tmp.lsb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  m[rr] = m[rr] | byte(mask_for_7th_bit);

  c.tick(4);
}

void cpu::SLA(reg8 r) noexcept {
  F.H(reset);
  F.N(reset);

  (r.msb() == 1) ? F.C(set) : F.C(reset);
  r = r.value() << 1U;
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

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

  r = r.value() >> 1U;
  r = r.value() | old_msb_mask;
  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == 0) ? F.Z(set) : F.Z(reset);

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
  r = r.value() >> 1;
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

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

  r = rotl(r.value(), 4);

  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SWAP(reg16 &rr) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  m[rr] = byte(rotl(std::to_integer<unsigned>(m[rr]), 4));

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
  r = r.value() | (1U << pos);

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

  r = r.value() & ~(1U << pos);

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

void cpu::RST(u8 t) noexcept {
  m[SP - 1] = PC.hi();
  m[SP - 2] = PC.lo();
  SP = SP - 2;

  PC.lo(byte(0b0000'0000));

  if (t == 0) {
    PC.hi(byte(0));
  } else if (t == 1) {
    PC.hi(byte(8));
  } else if (t == 2) {
    PC.hi(byte(10));
  } else if (t == 3) {
    PC.hi(byte(18));
  } else if (t == 4) {
    PC.hi(byte(20));
  } else if (t == 5) {
    PC.hi(byte(28));
  } else if (t == 6) {
    PC.hi(byte(30));
  } else if (t == 7) {
    PC.hi(byte(38));
  } else {
    /* (_8(|) -- D'oh! */
  }

  c.tick(4);
}

void cpu::DAA() noexcept {
  F.H(reset);

  if (F.N() == reset) {
    if (F.C() || A.value() > 0x99) {
      A += 0b0110'0000; // add 0x60 to get valid BCD
      F.C(set);
    }

    if (F.H() || A.loNibble() > 0x09) {
      A += 0b0000'0110;
    }
  } else {
    if (F.C()) {
      A -= 0b0110'0000;
    }

    if (F.H()) {
      A -= 0b0000'0110;
    }
  }

  (A.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(1);
}

void cpu::CPL() noexcept {
  F.H(set);
  F.N(set);

  A = ~A.value();
  c.tick(1);
}

void cpu::NOP() noexcept { c.tick(1); }

void cpu::CCF() noexcept {
  F.H(reset);
  F.N(reset);

  (F.C() == set) ? F.C(reset) : F.C(set);

  c.tick(1);
}

void cpu::SCF() noexcept {
  F.H(reset);
  F.N(reset);

  F.C(set);
  c.tick(1);
}

void cpu::DI() noexcept {
  IME = reset;
  c.tick(1);
}

void cpu::EI() noexcept {
  IME = set;
  c.tick(1);
}

void cpu::HALT() noexcept {
  // TODO: implement this
  c.tick(1);
}

void cpu::STOP() noexcept {
  // TODO: implement this
  c.tick(1);
}

// extracted from libstd++ <bit>
template <typename T> T cpu::rotl(T x, int s) noexcept {
  constexpr auto Nd = std::numeric_limits<T>::digits;
  const int r = s % Nd;
  if (r == 0) {
    return x;
  } else if (r > 0) {
    return (x << r) | (x >> ((Nd - r) % Nd));
  } else {
    return (x >> -r) | (x << ((Nd + r) % Nd)); // rotr(x, -r)
  }
}

template <typename T> T cpu::rotr(T x, int s) noexcept {
  constexpr auto Nd = std::numeric_limits<T>::digits;
  const int r = s % Nd;
  if (r == 0) {
    return x;
  } else if (r > 0) {
    return (x >> r) | (x << ((Nd - r) % Nd));
  } else {
    return (x << -r) | (x >> ((Nd + r) % Nd)); // rotl(x, -r)
  }
}

byte cpu::fetch(reg16 rr) { return m[rr]; }
u16 cpu::decode(byte b) { return std::to_integer<u16>(b); }

}

