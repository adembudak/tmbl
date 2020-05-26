#pragma once
#include "registers/reg16.h"
#include "registers/reg8.h"

namespace tmbl::cpu {

class cpu {
public:
  // return address
  int fetch();
  void decode();
  void execute(); // implements opcodes

private:
  // 8 bit transfer and I/O instructions
  //
  void LD(reg8 r1, reg8 r2);
  void LD(reg8 r, uint8 n);
  void LD(reg8 r1, reg16 r2);
  void LD(reg16 r1, reg8 r2);
  void LD(reg16 r, uint8 n);

private:
  reg16 PC;
  reg16 SP;

  reg8 A;
  reg8 F;

  reg16 BC;
  reg16 DE;
  reg16 HL;
};

}
