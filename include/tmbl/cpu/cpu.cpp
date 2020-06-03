#include "cpu.h"

namespace tmbl::cpu {

void cpu::LD(reg8 &r, reg16 &rr, u8 CY) noexcept {
  r = m[rr];
  PC += CY;
}

void cpu::LD(reg16 &rr, reg8 &r, u8 CY) noexcept {
  m[rr] = r.data();
  PC += CY;
}

void cpu::LD(reg16 &rr, u8 n, u8 CY) noexcept {
  m[rr] = n;
  PC += CY;
}

void cpu::LD(reg8 &r1, reg8 &r2, u8 CY, int dummy) noexcept {
  m[r1] = r2.data();
  PC += CY;
}

void cpu::LD(reg8 &r1, reg8 &r2, u8 CY) noexcept {
  r1 = r2;
  PC += CY;
}

void cpu::LD(reg8 &r, u8 n, u8 CY, int dummy) noexcept {
  r = m[n];
  PC += CY;
}

void cpu::LD(reg8 &r, u8 n, u8 CY) noexcept {
  r = n;
  PC += CY;
}

void cpu::LD(u8 n, reg8 r, u8 CY) noexcept {
  m[n] = r.data();
  PC += CY;
}

void cpu::LD(reg8 &r, u16 nn, u8 CY) noexcept {
  r = m[nn];
  PC += CY;
}

void cpu::LD(u16 nn, reg8 &r, u8 CY) noexcept {
  m[nn] = r.data();
  PC += CY;
}

void cpu::LD(u16 nn, reg16 &rr, u8 CY) noexcept {
  m[nn] = rr.data();
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

void cpu::run() {
  for (;;) {

    //    switch (byte addr = fetch(); addr) {
    // decode and execute
    //    case 0x00:
    //     NOP();
    //   case 0x01:
  }
};
}

