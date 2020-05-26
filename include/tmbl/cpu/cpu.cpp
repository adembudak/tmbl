#include "cpu.h"
namespace tmbl::cpu {

void cpu::LD(reg8 r1, reg8 r2) {
  r1 = r2;
  PC += 1;
}

void cpu::LD(reg8 r, uint8 n) {
  r = n;
  PC += 2;
}

void cpu::LD(reg8 r1, reg16 r2) {
  r1 = r2;
  PC += 2;
}

void cpu::LD(reg16 r1, reg8 r2) {
  r1 = r2;
  PC += 2;
}

void cpu::LD(reg16 r, uint8 n) {
  r = n;
  PC += 3;
}

}
