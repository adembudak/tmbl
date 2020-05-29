#include "tmbl/cpu/cpu.h"
#include <gtest/gtest.h>

using namespace tmbl;
using namespace tmbl::cpu;

tmbl::cpu::cpu c;
reg8 A;
reg8 F;

reg16 HL;

uint8 n = 0b1010'0001;
uint16 nn = 0b0000'0000'1111'1101;

// LD(r r')
// 0x7f
// 0x40
// 0x49
// 0x6D
// 0x5B
// 0x64
TEST(Instructions, LD) {
  F = n;
  c.LD(A, F, 1);

  ASSERT_EQ(A, F);
}

// LD (r, n)
// 0x3E
// 0x06
// 0x0E
// 0x2E
// 0x1E
// 0x26
TEST(Instructions, LD2) {
  c.LD(A, n, 2);

  ASSERT_EQ(A.data(), n);
}

// LD(r, (HL))
// 0x7E
// 0x46
// 0x4E
// 0x6E
// 0x5E
// 0x66
TEST(Instructions, LD3) {
  nn = 0b0000'0000'1101'0001;
  HL = nn;
  c.LD(A, HL, 2);

  ASSERT_EQ(A.data(), HL.data());
}

// LD((HL), r)
// 0x77
// 0x70
// 0x71
// 0x75
// 0x73
// 0x74
TEST(Instructions, LD4) {
  // arbitrary 16 bit value
  nn = 0b0000'0000'1101'1011;
  HL = nn;
  c.LD(HL, A, 2);

  ASSERT_EQ(HL.data(), A.data());
}

// LD((HL), n)
// 0x36
TEST(Instructions, LD5) {
  n = 0b1111'0000;
  c.LD(HL, n, 3);
  ASSERT_EQ(HL.data(), n);
}

// LD(A, (BC))
// 0x0A
// Assumption: BC is lower than 255
TEST(Instructions, LD6) {
  nn = 0b0000'0000'1111'0000;
  reg16 BC;
  BC = nn;
  c.LD(A, BC, 2);
  ASSERT_EQ(A.data(), BC.data());
}

