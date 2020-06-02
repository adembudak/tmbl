#include "tmbl/cpu/cpu.h"
#include <gtest/gtest.h>

using namespace tmbl;
using namespace tmbl::cpu;

// TODO Fix the LD instruction

tmbl::cpu::cpu c;
reg8 A;
reg8 F;

reg16 HL;

u8 n = 0b1010'0001;
u16 nn = 0b0000'0000'1111'1101;

// LD(r r')
// 0x7f
// 0x40
// 0x49
// 0x6D
// 0x5B
// 0x64
TEST(Instructions, LD) { ASSERT_EQ(1, 1); }

// LD (r, n)
// 0x3E
// 0x06
// 0x0E
// 0x2E
// 0x1E
// 0x26
TEST(Instructions, LD2) { ASSERT_EQ(1, 1); }

// LD(r, (HL))
// 0x7E
// 0x46
// 0x4E
// 0x6E
// 0x5E
// 0x66
TEST(Instructions, LD3) { ASSERT_EQ(1, 1); }

// LD((HL), r)
// 0x77
// 0x70
// 0x71
// 0x75
// 0x73
// 0x74
TEST(Instructions, LD4) { ASSERT_EQ(1, 1); }

// LD((HL), n)
// 0x36
TEST(Instructions, LD5) { ASSERT_EQ(1, 1); }

// LD(A, (BC))
// 0x0A
TEST(Instructions, LD6) { ASSERT_EQ(1, 1); }

