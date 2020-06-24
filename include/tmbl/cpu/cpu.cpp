#include "cpu.h"
#include <stdexcept>

namespace tmbl::cpu {
cpu::cpu(std::shared_ptr<bus::bus> b) : bus(std::move(b)) {}

void cpu::run() {
  // clang-format off
  auto make_i8 = [](const byte b) -> i8 { return b; };
  auto make_u8 = [](const byte b) -> u8 { return b; };
  auto make_u16 = [](const byte l, const byte u) -> u16 { return static_cast<u16>((u << 8) | l);};
  // clang-format on

  u16 nn = 0;
  u8 n = 0;
  i8 e = 0;

  reg16 AF;
  reg8 D, E;
  reg8 B, C;
  reg8 H, L;

  for (;;) {
    // clang-format off
    D = DE.hi(); E = DE.lo();
    B = BC.hi(); C = BC.lo();
    H = HL.hi(); L = HL.lo();

    AF.hi(A); AF.lo(F);
    // clang-format on

    switch (fetch(PC++)) {
      case 0x00:
        NOP();
        break;

      case 0x01:
        nn = make_u16(fetch(PC++), fetch(PC++));
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
        BC.hi(B);
        break;

      case 0x07:
        RLCA();
        break;

      case 0x08:
        nn = make_u16(fetch(PC++), fetch(PC++));
        LD(nn, SP);
        break;

      case 0x09:
        ADD(HL, BC);
        break;

      case 0x0A:
        LD(A, BC);
        break;

      case 0x0B:
        DEC(BC);
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
        nn = make_u16(fetch(PC++), fetch(PC++));
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
        DEC(DE);
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

      case 0x20:
        e = make_i8(fetch(PC++));
        JR(00, e);
        break;

      case 0x21:
        nn = make_u16(fetch(PC++), fetch(PC++));
        LD(HL, nn);
        break;

      case 0x22:
        LDi();
        break;

      case 0x23:
        INC(HL);
        break;

      case 0x24:
        INC(H);
        HL.hi(H);
        break;

      case 0x25:
        DEC(H);
        HL.hi(H);
        break;

      case 0x26:
        n = make_u8(fetch(PC++));
        LD(H, n);
        HL.hi(H);
        break;

      case 0x27:
        DAA();
        break;

      case 0x28:
        e = make_i8(fetch(PC++));
        JR(1, e);
        break;

      case 0x29:
        ADD(HL, HL);
        break;

      case 0x2A:
        LDi(/*dummy*/ 666);
        break;

      case 0x2B:
        DEC(HL);
        break;

      case 0x2C:
        INC(L);
        HL.lo(L);
        break;

      case 0x2D:
        DEC(L);
        HL.lo(L);
        break;

      case 0x2E:
        n = make_u8(fetch(PC++));
        LD(L, n);
        break;
      case 0x2F:
        CPL();
        break;

      case 0x30:
        e = make_i8(fetch(PC++));
        JR(10, e);

        break;

      case 0x31:
        nn = make_u16(fetch(PC++), fetch(PC++));
        LD(SP, nn);
        break;

      case 0x32:
        LDi();
        break;

      case 0x33:
        INC(SP);
        break;

      case 0x34:
        INC();
        break;

      case 0x35:
        DEC();
        break;

      case 0x36:
        n = make_u8(fetch(PC++));
        LD(HL, n, /*dummy*/ 666);
        break;

      case 0x37:
        SCF();
        break;

      case 0x38:
        e = make_i8(fetch(PC++));
        JR(11, e);
        break;

      case 0x39:
        ADD(HL, SP);
        break;

      case 0x3A:
        LDd(/*dummy*/ 666);
        break;

      case 0x3B:
        DEC(SP);
        break;

      case 0x3C:
        INC(A);
        break;

      case 0x3D:
        DEC(A);
        break;

      case 0x3E:
        n = make_u8(fetch(PC++));
        LD(A, n);
        break;

      case 0x3F:
        CCF();
        break;

      case 0x40:
        LD(B, B);
        BC.hi(B);
        break;

      case 0x41:
        LD(B, C);
        BC.hi(B);
        break;

      case 0x42:
        LD(B, D);
        BC.hi(B);
        break;

      case 0x43:
        LD(B, E);
        BC.hi(B);
        break;

      case 0x44:
        LD(B, H);
        BC.hi(B);
        break;

      case 0x45:
        LD(B, L);
        BC.hi(B);
        break;

      case 0x46:
        LD(B, HL);
        BC.hi(B);
        break;

      case 0x47:
        LD(B, A);
        BC.hi(B);
        break;

      case 0x48:
        LD(C, B);
        BC.lo(C);
        break;

      case 0x49:
        LD(C, C);
        BC.lo(C);
        break;

      case 0x4A:
        LD(C, D);
        BC.lo(C);
        break;

      case 0x4B:
        LD(C, E);
        BC.lo(C);
        break;

      case 0x4C:
        LD(C, H);
        BC.lo(C);
        break;

      case 0x4D:
        LD(C, L);
        BC.lo(C);
        break;

      case 0x4E:
        LD(C, HL);
        BC.lo(C);
        break;

      case 0x4F:
        LD(C, A);
        BC.lo(C);
        break;

      case 0x50:
        LD(D, B);
        DE.hi(D);
        break;

      case 0x51:
        LD(D, C);
        DE.hi(D);
        break;

      case 0x52:
        LD(D, D);
        DE.hi(D);
        break;

      case 0x53:
        LD(D, E);
        DE.hi(E);
        break;

      case 0x54:
        LD(D, H);
        DE.hi(D);
        break;

      case 0x55:
        LD(D, L);
        DE.hi(D);
        break;

      case 0x56:
        LD(D, HL);
        DE.hi(D);
        break;

      case 0x57:
        LD(D, A);
        DE.hi(D);
        break;

      case 0x58:
        LD(E, B);
        DE.hi(E);
        break;

      case 0x59:
        LD(E, C);
        DE.hi(E);
        break;

      case 0x5A:
        LD(E, D);
        DE.hi(E);
        break;

      case 0x5B:
        LD(E, E);
        DE.hi(E);
        break;

      case 0x5C:
        LD(E, H);
        DE.hi(E);
        break;

      case 0x5D:
        LD(E, L);
        DE.hi(E);
        break;

      case 0x5E:
        LD(E, HL);
        DE.hi(E);
        break;

      case 0x5F:
        LD(E, A);
        DE.hi(E);
        break;

      case 0x60:
        LD(H, B);
        HL.hi(H);
        break;

      case 0x61:
        LD(H, C);
        HL.hi(H);
        break;

      case 0x62:
        LD(H, D);
        HL.hi(H);
        break;

      case 0x63:
        LD(H, E);
        HL.hi(H);
        break;

      case 0x64:
        LD(H, H);
        HL.hi(H);
        break;

      case 0x65:
        LD(H, L);
        HL.hi(H);
        break;

      case 0x66:
        LD(H, HL);
        HL.hi(H);
        break;

      case 0x67:
        LD(H, A);
        HL.hi(H);
        break;

      case 0x68:
        LD(L, B);
        HL.lo(L);
        break;

      case 0x69:
        LD(L, C);
        HL.lo(L);
        break;

      case 0x6A:

        LD(L, D);
        HL.lo(L);
        break;

      case 0x6B:
        LD(L, E);
        HL.lo(L);
        break;

      case 0x6C:
        LD(L, H);
        HL.lo(L);
        break;

      case 0x6D:
        LD(L, L);
        HL.lo(L);
        break;

      case 0x6E:
        LD(L, HL);
        HL.lo(L);
        break;

      case 0x6F:
        LD(L, A);
        HL.lo(L);
        break;

      case 0x70:
        LD(HL, B);
        break;

      case 0x71:
        LD(HL, C);
        break;

      case 0x72:
        LD(HL, D);
        break;

      case 0x73:
        LD(HL, E);
        break;

      case 0x74:
        LD(HL, H);
        break;

      case 0x75:
        LD(HL, L);
        break;

      case 0x76:
        HALT();
        break;

      case 0x77:
        LD(HL, A);
        break;

      case 0x78:
        LD(A, B);
        break;

      case 0x79:
        LD(A, C);
        break;

      case 0x7A:
        LD(A, D);
        break;

      case 0x7B:
        LD(A, E);
        break;

      case 0x7C:
        LD(A, H);
        break;

      case 0x7D:
        LD(A, L);
        break;

      case 0x7E:
        LD(A, HL);
        break;

      case 0x7F:
        LD(A, A);
        break;

      case 0x80:
        ADD(B);
        break;

      case 0x81:
        ADD(C);
        break;

      case 0x82:
        ADD(D);
        break;

      case 0x83:
        ADD(E);
        break;

      case 0x84:
        ADD(H);
        break;

      case 0x85:
        ADD(L);
        break;

      case 0x86:
        ADD(HL);
        break;

      case 0x87:
        ADD(A);
        break;

      case 0x88:
        ADC(B);
        break;

      case 0x89:
        ADC(C);
        break;

      case 0x8A:
        ADC(D);
        break;

      case 0x8B:
        ADC(E);
        break;

      case 0x8C:
        ADC(H);
        break;

      case 0x8D:
        ADC(L);
        break;

      case 0x8E:
        ADC(HL);
        break;

      case 0x8F:
        ADC(A);
        break;

      case 0x90:
        SUB(B);
        break;

      case 0x91:
        SUB(C);
        break;

      case 0x92:
        SUB(D);
        break;

      case 0x93:
        SUB(E);
        break;

      case 0x94:
        SUB(H);
        break;

      case 0x95:
        SUB(L);
        break;

      case 0x96:
        SUB(HL);
        break;

      case 0x97:
        SUB(A);
        break;

      case 0x98:
        SBC(B);
        break;

      case 0x99:
        SBC(C);
        break;

      case 0x9A:
        SBC(D);
        break;

      case 0x9B:
        SBC(E);
        break;

      case 0x9C:
        SBC(H);
        break;

      case 0x9D:
        SBC(L);
        break;

      case 0x9E:
        SBC(HL);
        break;

      case 0x9F:
        SBC(A);
        break;

      case 0xA0:
        AND(B);
        break;

      case 0xA1:
        AND(C);
        break;

      case 0xA2:
        AND(D);
        break;

      case 0xA3:
        AND(E);
        break;

      case 0xA4:
        AND(H);
        break;

      case 0xA5:
        AND(L);
        break;

      case 0xA6:
        AND(HL);
        break;

      case 0xA7:
        AND(A);
        break;

      case 0xA8:
        XOR(B);
        break;

      case 0xA9:
        XOR(C);
        break;

      case 0xAA:
        XOR(D);
        break;

      case 0xAB:
        XOR(E);
        break;

      case 0xAC:
        XOR(H);
        break;

      case 0xAD:
        XOR(L);
        break;

      case 0xAE:
        XOR(HL);
        break;

      case 0xAF:
        XOR(A);
        break;

      case 0xB0:
        OR(B);
        break;

      case 0xB1:
        OR(C);
        break;

      case 0xB2:
        OR(D);
        break;

      case 0xB3:
        OR(E);
        break;

      case 0xB4:
        OR(H);
        break;

      case 0xB5:
        OR(L);
        break;

      case 0xB6:
        OR(HL);
        break;

      case 0xB7:
        OR(A);
        break;

      case 0xB8:
        CP(B);
        break;

      case 0xB9:
        CP(C);
        break;

      case 0xBA:
        CP(D);
        break;

      case 0xBB:
        CP(E);
        break;

      case 0xBC:
        CP(H);
        break;

      case 0xBD:
        CP(L);
        break;

      case 0xBE:
        CP(HL);
        break;

      case 0xBF:
        CP(A);
        break;

      case 0xC0:
        RET(0);
        break;

      case 0xC1:
        POP(BC);
        break;

      case 0xC2:
        nn = make_u16(fetch(PC++), fetch(PC++));
        JP(/*NZ*/ 0, nn);
        break;

      case 0xC3:
        nn = make_u16(fetch(PC++), fetch(PC++));
        JP(nn);
        break;

      case 0xC4:
        nn = make_u16(fetch(PC++), fetch(PC++));
        CALL(0, nn);
        break;

      case 0xC5:
        PUSH(BC);
        break;

      case 0xC6:
        n = make_u8(fetch(PC++));
        ADD(n);
        break;

      case 0xC7:
        RST(0);
        break;

      case 0xC8:
        RST(/*Z*/ 1);
        break;

      case 0xC9:
        RET();
        break;

      case 0xCA:
        nn = make_u16(fetch(PC++), fetch(PC++));
        JP(/*Z*/ 1, nn);
        break;

      case 0xCB:
        switch (fetch(PC++)) {
          case 0x00:
            RLC(B);
            BC.hi(B);
            break;

          case 0x01:
            RLC(C);
            BC.lo(C);
            break;

          case 0x02:
            RLC(D);
            DE.hi(D);
            break;
          case 0x03:
            RLC(E);
            DE.lo(E);
            break;

          case 0x04:
            RLC(H);
            HL.hi(H);
            break;

          case 0x05:
            RLC(L);
            HL.lo(L);
            break;

          case 0x06:
            RLC(HL);
            break;

          case 0x07:
            RLC(C);
            BC.lo(C);
            break;

          case 0x08:
            RRC(B);
            BC.hi(B);
            break;

          case 0x09:
            RRC(C);
            BC.lo(C);
            break;

          case 0x0A:
            RRC(D);
            DE.hi(D);
            break;

          case 0x0B:
            RRC(E);
            DE.lo(E);
            break;

          case 0x0C:
            RRC(H);
            HL.hi(H);
            break;

          case 0x0D:
            RRC(L);
            HL.lo(L);
            break;

          case 0x0E:
            RRC(HL);
            break;

          case 0x0F:
            RRC(A);
            AF.hi(A);
            break;

          case 0x10:
            RL(B);
            BC.hi(B);
            break;

          case 0x11:
            RL(C);
            BC.lo(C);
            break;

          case 0x12:
            RL(D);
            DE.hi(D);
            break;

          case 0x13:
            RL(E);
            DE.lo(E);
            break;

          case 0x14:
            RL(H);
            HL.hi(H);
            break;
          case 0x15:
            RL(L);
            HL.lo(L);
            break;

          case 0x16:
            RL(HL);
            break;

          case 0x17:
            RL(A);
            AF.hi(A);
            break;

          case 0x18:
            RR(B);
            BC.hi(B);
            break;

          case 0x19:
            RR(C);
            BC.lo(C);
            break;

          case 0x1A:
            RR(D);
            DE.hi(D);
            break;

          case 0x1B:
            RR(E);
            DE.lo(E);
            break;

          case 0x1C:
            RR(H);
            HL.hi(H);
            break;

          case 0x1D:
            RR(L);
            HL.lo(L);
            break;

          case 0x1E:
            RR(HL);
            break;

          case 0x1F:
            RR(A);
            AF.hi(A);
            break;

          case 0x20:
            SLA(B);
            BC.hi(B);
            break;

          case 0x21:
            SLA(C);
            BC.lo(C);
            break;

          case 0x22:
            SLA(D);
            DE.hi(D);
            break;

          case 0x23:
            SLA(E);
            DE.lo(E);
            break;

          case 0x24:
            SLA(H);
            HL.hi(H);
            break;

          case 0x25:
            SLA(L);
            HL.lo(L);
            break;

          case 0x26:
            SLA(HL);
            break;

          case 0x27:
            SLA(A);
            AF.hi(A);
            break;

          case 0x28:
            SRA(B);
            BC.hi(B);
            break;

          case 0x29:
            SRA(C);
            BC.lo(C);
            break;

          case 0x2A:
            SRA(D);
            DE.hi(D);
            break;

          case 0x2B:
            SRA(E);
            DE.lo(E);
            break;

          case 0x2C:
            SRA(H);
            HL.hi(H);
            break;

          case 0x2D:
            SRA(L);
            HL.lo(L);
            break;

          case 0x2E:
            SRA(HL);
            break;

          case 0x2F:
            SRA(A);
            AF.hi(A);
            break;

          case 0x30:
            SWAP(B);
            BC.hi(B);
            break;

          case 0x31:
            SWAP(C);
            BC.lo(C);
            break;

          case 0x32:
            SWAP(D);
            DE.hi(D);
            break;

          case 0x33:
            SWAP(E);
            DE.lo(E);
            break;

          case 0x34:
            SWAP(H);
            HL.hi(H);
            break;

          case 0x35:
            SWAP(L);
            HL.lo(L);
            break;

          case 0x36:
            SWAP(HL);
            break;

          case 0x37:
            SWAP(A);
            break;

          case 0x38:
            SRL(B);
            BC.hi(B);
            break;

          case 0x39:
            SRL(C);
            BC.lo(C);
            break;

          case 0x3A:
            SRL(D);
            DE.hi(D);
            break;

          case 0x3B:
            SRL(E);
            DE.lo(E);
            break;

          case 0x3C:
            SRL(H);
            HL.hi(H);
            break;

          case 0x3D:
            SRL(L);
            HL.lo(L);
            break;

          case 0x3E:
            SRL(HL);
            break;

          case 0x3F:
            SRL(A);
            break;

          case 0x40:
            BIT(B, 0);
            BC.hi(B);
            break;

          case 0x41:
            BIT(C, 0);
            BC.lo(C);
            break;

          case 0x42:
            BIT(D, 0);
            DE.hi(D);
            break;

          case 0x43:
            BIT(E, 0);
            DE.lo(E);
            break;

          case 0x44:
            BIT(H, 0);
            HL.hi(H);
            break;

          case 0x45:
            BIT(L, 0);
            HL.lo(L);
            break;

          case 0x46:
            BIT(HL, 0);
            break;

          case 0x47:
            BIT(A, 0);
            break;

          case 0x48:
            BIT(B, 1);
            BC.hi(B);
            break;

          case 0x49:
            BIT(C, 1);
            BC.lo(C);
            break;

          case 0x4A:
            BIT(D, 1);
            DE.hi(D);
            break;

          case 0x4B:
            BIT(E, 1);
            DE.lo(E);
            break;

          case 0x4C:
            BIT(H, 1);
            HL.hi(H);
            break;

          case 0x4D:
            BIT(L, 1);
            HL.lo(L);
            break;

          case 0x4E:
            BIT(HL, 1);
            break;

          case 0x4F:
            BIT(A, 1);
            break;

          case 0x50:
            BIT(B, 2);
            BC.hi(B);
            break;

          case 0x51:
            BIT(C, 2);
            BC.lo(C);
            break;

          case 0x52:
            BIT(D, 2);
            DE.hi(D);
            break;

          case 0x53:
            BIT(E, 2);
            DE.lo(E);
            break;

          case 0x54:
            BIT(H, 2);
            HL.hi(H);
            break;

          case 0x55:
            BIT(L, 2);
            HL.lo(L);
            break;

          case 0x56:
            BIT(HL, 2);
            break;

          case 0x57:
            BIT(A, 2);
            break;

          case 0x58:
            BIT(B, 3);
            BC.hi(B);
            break;

          case 0x59:
            BIT(C, 3);
            BC.lo(C);
            break;

          case 0x5A:
            BIT(D, 3);
            DE.hi(D);
            break;

          case 0x5B:
            BIT(E, 3);
            DE.lo(E);
            break;

          case 0x5C:
            BIT(H, 3);
            HL.hi(H);
            break;

          case 0x5D:
            BIT(L, 3);
            HL.lo(L);
            break;

          case 0x5E:
            BIT(HL, 3);
            break;

          case 0x5F:
            BIT(A, 3);
            break;

          case 0x60:
            BIT(B, 4);
            BC.hi(B);
            break;

          case 0x61:
            BIT(C, 4);
            BC.lo(C);
            break;

          case 0x62:
            BIT(D, 4);
            DE.hi(D);
            break;

          case 0x63:
            BIT(E, 4);
            DE.lo(E);
            break;

          case 0x64:
            BIT(H, 4);
            HL.hi(H);
            break;

          case 0x65:
            BIT(L, 4);
            HL.lo(L);
            break;

          case 0x66:
            BIT(HL, 4);
            break;

          case 0x67:
            BIT(A, 4);
            break;

          case 0x68:
            BIT(B, 5);
            BC.hi(B);
            break;

          case 0x69:
            BIT(C, 5);
            BC.lo(C);
            break;

          case 0x6A:
            BIT(D, 5);
            DE.hi(D);
            break;

          case 0x6B:
            BIT(E, 5);
            DE.lo(E);
            break;

          case 0x6C:
            BIT(H, 5);
            HL.hi(H);
            break;

          case 0x6D:
            BIT(L, 5);
            HL.lo(L);
            break;

          case 0x6E:
            BIT(HL, 5);
            break;

          case 0x6F:
            BIT(A, 5);
            break;

          case 0x70:
            BIT(B, 6);
            BC.hi(B);
            break;

          case 0x71:
            BIT(C, 6);
            BC.lo(C);
            break;

          case 0x72:
            BIT(D, 6);
            DE.hi(D);
            break;

          case 0x73:
            BIT(E, 6);
            DE.lo(E);
            break;

          case 0x74:
            BIT(H, 6);
            HL.hi(H);
            break;

          case 0x75:
            BIT(L, 6);
            HL.lo(L);
            break;

          case 0x76:
            BIT(HL, 6);
            break;

          case 0x77:
            BIT(A, 6);
            break;

          case 0x78:
            BIT(B, 7);
            BC.hi(B);
            break;

          case 0x79:
            BIT(C, 7);
            BC.lo(C);
            break;

          case 0x7A:
            BIT(D, 7);
            DE.hi(D);
            break;

          case 0x7B:
            BIT(E, 7);
            DE.lo(E);
            break;

          case 0x7C:
            BIT(H, 7);
            HL.hi(H);
            break;

          case 0x7D:
            BIT(L, 7);
            HL.lo(L);
            break;

          case 0x7E:
            BIT(HL, 7);
            break;

          case 0x7F:
            BIT(A, 7);
            break;

          case 0x80:
            RES(B, 0);
            BC.hi(B);
            break;

          case 0x81:
            RES(C, 0);
            BC.lo(C);
            break;

          case 0x82:
            RES(D, 0);
            DE.hi(D);
            break;

          case 0x83:
            RES(E, 0);
            DE.lo(E);
            break;

          case 0x84:
            RES(H, 0);
            HL.hi(H);
            break;

          case 0x85:
            RES(L, 0);
            HL.lo(L);
            break;

          case 0x86:
            RES(HL, 0);
            break;

          case 0x87:
            RES(A, 0);
            break;

          case 0x88:
            RES(B, 1);
            BC.hi(B);
            break;

          case 0x89:
            RES(C, 1);
            BC.lo(C);
            break;

          case 0x8A:
            RES(D, 1);
            DE.hi(D);
            break;

          case 0x8B:
            RES(E, 1);
            DE.lo(E);
            break;

          case 0x8C:
            RES(H, 1);
            HL.hi(H);
            break;

          case 0x8D:
            RES(L, 1);
            HL.lo(L);
            break;

          case 0x8E:
            RES(HL, 1);
            break;

          case 0x8F:
            RES(A, 1);
            break;

          case 0x90:
            RES(B, 2);
            BC.hi(B);
            break;

          case 0x91:
            RES(C, 2);
            BC.lo(C);
            break;

          case 0x92:
            RES(D, 2);
            DE.hi(D);
            break;

          case 0x93:
            RES(E, 2);
            DE.lo(E);
            break;

          case 0x94:
            RES(H, 2);
            HL.hi(H);
            break;

          case 0x95:
            RES(L, 2);
            HL.lo(L);
            break;

          case 0x96:
            RES(HL, 2);
            break;

          case 0x97:
            RES(A, 2);
            break;

          case 0x98:
            RES(B, 3);
            BC.hi(B);
            break;

          case 0x99:
            RES(C, 3);
            BC.lo(C);
            break;

          case 0x9A:
            RES(D, 3);
            DE.hi(D);
            break;

          case 0x9B:
            RES(E, 3);
            DE.lo(E);
            break;

          case 0x9C:
            RES(H, 2);
            HL.hi(H);
            break;

          case 0x9D:
            RES(L, 3);
            HL.lo(L);
            break;

          case 0x9E:
            RES(HL, 3);
            break;

          case 0x9F:
            RES(A, 3);
            break;

          case 0xA0:
            RES(B, 4);
            BC.hi(B);
            break;

          case 0xA1:
            RES(C, 4);
            BC.lo(C);
            break;

          case 0xA2:
            RES(D, 4);
            DE.hi(D);
            break;

          case 0xA3:
            RES(E, 4);
            DE.lo(E);
            break;

          case 0xA4:
            RES(H, 4);
            HL.hi(H);
            break;

          case 0xA5:
            RES(L, 4);
            HL.lo(L);
            break;

          case 0xA6:
            RES(HL, 4);
            break;

          case 0xA7:
            RES(A, 4);
            break;

          case 0xA8:
            RES(B, 5);
            BC.hi(B);
            break;

          case 0xA9:
            RES(C, 5);
            BC.lo(C);
            break;

          case 0xAA:
            RES(D, 5);
            DE.hi(D);
            break;

          case 0xAB:
            RES(E, 5);
            DE.lo(E);
            break;

          case 0xAC:
            RES(H, 5);
            HL.hi(H);
            break;

          case 0xAD:
            RES(L, 5);
            HL.lo(L);
            break;

          case 0xAE:
            RES(HL, 5);
            break;

          case 0xAF:
            RES(A, 5);
            break;

          case 0xB0:
            RES(B, 6);
            BC.hi(B);
            break;

          case 0xB1:
            RES(C, 6);
            BC.lo(C);
            break;

          case 0xB2:
            RES(D, 6);
            DE.hi(D);
            break;

          case 0xB3:
            RES(E, 6);
            DE.lo(E);
            break;

          case 0xB4:
            RES(H, 6);
            HL.hi(H);
            break;

          case 0xB5:
            RES(L, 6);
            HL.lo(L);
            break;

          case 0xB6:
            RES(HL, 6);
            break;

          case 0xB7:
            RES(A, 6);
            break;

          case 0xB8:
            RES(B, 7);
            BC.hi(B);
            break;

          case 0xB9:
            RES(C, 7);
            BC.lo(C);
            break;

          case 0xBA:
            RES(D, 7);
            DE.hi(D);
            break;

          case 0xBB:
            RES(E, 7);
            DE.lo(E);
            break;

          case 0xBC:
            RES(H, 7);
            HL.hi(H);
            break;

          case 0xBD:
            RES(L, 7);
            HL.lo(L);
            break;

          case 0xBE:
            RES(HL, 7);
            break;

          case 0xBF:
            RES(A, 7);
            break;

          case 0xC0:
            SET(B, 0);
            BC.hi(B);
            break;

          case 0xC1:
            SET(C, 0);
            BC.lo(C);
            break;

          case 0xC2:
            SET(D, 0);
            DE.hi(D);
            break;

          case 0xC3:
            SET(E, 0);
            DE.lo(E);
            break;

          case 0xC4:
            SET(H, 0);
            HL.hi(H);
            break;

          case 0xC5:
            SET(L, 0);
            HL.lo(L);
            break;

          case 0xC6:
            SET(HL, 0);
            break;

          case 0xC7:
            SET(A, 0);
            break;

          case 0xC8:
            SET(B, 1);
            BC.hi(B);
            break;

          case 0xC9:
            SET(C, 1);
            BC.lo(C);
            break;

          case 0xCA:
            SET(D, 1);
            DE.hi(D);
            break;

          case 0xCB:
            SET(E, 1);
            DE.lo(E);
            break;

          case 0xCC:
            SET(H, 1);
            HL.hi(H);
            break;

          case 0xCD:
            SET(L, 1);
            HL.lo(L);
            break;

          case 0xCE:
            SET(HL, 1);
            break;

          case 0xCF:
            SET(A, 1);
            break;

          case 0xD0:
            SET(B, 2);
            BC.hi(B);
            break;

          case 0xD1:
            SET(C, 2);
            BC.lo(C);
            break;

          case 0xD2:
            SET(D, 2);
            DE.hi(D);
            break;

          case 0xD3:
            SET(E, 2);
            DE.lo(E);
            break;

          case 0xD4:
            SET(H, 2);
            HL.hi(H);
            break;

          case 0xD5:
            SET(L, 2);
            HL.lo(L);
            break;

          case 0xD6:
            SET(HL, 2);
            break;

          case 0xD7:
            SET(A, 2);
            break;

          case 0xD8:
            SET(B, 3);
            BC.hi(B);
            break;

          case 0xD9:
            SET(C, 3);
            BC.lo(C);
            break;

          case 0xDA:
            SET(D, 3);
            DE.hi(D);
            break;

          case 0xDB:
            SET(E, 3);
            DE.lo(E);
            break;

          case 0xDC:
            SET(H, 3);
            HL.hi(H);
            break;

          case 0xDD:
            SET(L, 3);
            HL.lo(L);
            break;

          case 0xDE:
            SET(HL, 3);
            break;

          case 0xDF:
            SET(A, 3);
            break;

          case 0xE0:
            SET(B, 4);
            BC.hi(B);
            break;

          case 0xE1:
            SET(C, 4);
            BC.lo(C);
            break;

          case 0xE2:
            SET(D, 4);
            DE.hi(D);
            break;

          case 0xE3:
            SET(E, 4);
            DE.lo(E);
            break;

          case 0xE4:
            SET(H, 4);
            HL.hi(H);
            break;

          case 0xE5:
            SET(L, 4);
            HL.lo(L);
            break;

          case 0xE6:
            SET(HL, 4);
            break;

          case 0xE7:
            SET(A, 4);
            break;

          case 0xE8:
            SET(B, 5);
            BC.hi(B);
            break;

          case 0xE9:
            SET(C, 5);
            BC.lo(C);
            break;

          case 0xEA:
            SET(D, 5);
            DE.hi(D);
            break;

          case 0xEB:
            SET(E, 5);
            DE.lo(E);
            break;

          case 0xEC:
            SET(H, 5);
            HL.hi(H);
            break;

          case 0xED:
            SET(L, 5);
            HL.lo(L);
            break;

          case 0xEE:
            SET(HL, 5);
            break;

          case 0xEF:
            SET(A, 5);
            break;

          case 0xF0:
            SET(B, 6);
            BC.hi(B);
            break;

          case 0xF1:
            SET(C, 6);
            BC.lo(C);
            break;

          case 0xF2:
            SET(D, 6);
            DE.hi(D);
            break;

          case 0xF3:
            SET(E, 6);
            DE.lo(E);
            break;

          case 0xF4:
            SET(H, 6);
            HL.hi(H);
            break;

          case 0xF5:
            SET(L, 6);
            HL.lo(L);
            break;

          case 0xF6:
            SET(HL, 6);
            break;

          case 0xF7:
            SET(A, 6);
            break;

          case 0xF8:
            SET(B, 7);
            BC.hi(B);
            break;

          case 0xF9:
            SET(C, 7);
            BC.lo(C);
            break;

          case 0xFA:
            SET(D, 7);
            DE.hi(D);
            break;

          case 0xFB:
            SET(E, 7);
            DE.lo(E);
            break;

          case 0xFC:
            SET(H, 7);
            HL.hi(H);
            break;

          case 0xFD:
            SET(L, 7);
            HL.lo(L);
            break;

          case 0xFE:
            SET(HL, 7);
            break;

          case 0xFF:
            SET(A, 7);
            break;
        }

      case 0xCC:
        nn = make_u16(fetch(PC++), fetch(PC++));
        CALL(/*Z*/ 1, nn);
        break;

      case 0xCD:
        nn = make_u16(fetch(PC++), fetch(PC++));
        CALL(nn);
        break;

      case 0xCE:
        n = make_u8(fetch(PC++));
        ADC(n);
        break;

      case 0xCF:
        RST(8);
        break;

      case 0xD0:
        RET(10);
        break;

      case 0xD1:
        POP(DE);
        break;

      case 0xD2:
        nn = make_u16(fetch(PC++), fetch(PC++));
        JP(10, nn);
        break;

      case 0xD3:
        break;

      case 0xD4:
        nn = make_u16(fetch(PC++), fetch(PC++));
        CALL(10, nn);
        break;

      case 0xD5:
        PUSH(DE);
        break;

      case 0xD6:
        n = make_u8(fetch(PC++));
        SUB(n);
        break;

      case 0xD7:
        RST(10);
        break;

      case 0xD8:
        RET(11);
        break;

      case 0xD9:
        RETI();
        break;

      case 0xDA:
        nn = make_u16(fetch(PC++), fetch(PC++));
        JP(/*C*/ 11, nn);
        break;

      case 0xDB:
        break;

      case 0xDC:
        nn = make_u16(fetch(PC++), fetch(PC++));
        CALL(/*C*/ 11, nn);
        break;

      case 0xDD:
        break;

      case 0xDE:
        n = make_u8(fetch(PC++));
        SBC(n);
        break;

      case 0xDF:
        RST(18);
        break;

      case 0xE0:
        n = make_u8(fetch(PC++));
        LD(n, Orientation::WRITE_TO_IO_PORT);
        break;

      case 0xE1:
        POP(HL);
        break;

      case 0xE2:
        LD(Orientation::WRITE_TO_IO_PORT);
        break;

      case 0xE3:
        break;

      case 0xE4:
        break;

      case 0xE5:
        PUSH(HL);
        break;

      case 0xE6:
        n = make_u8(fetch(PC++));
        AND(n);
        break;

      case 0xE7:
        RST(20);
        break;

      case 0xE8:
        e = make_i8(fetch(PC++));
        ADD(SP, e);
        break;

      case 0xE9:
        JP(HL);
        break;

      case 0xEA:
        nn = make_u16(fetch(PC++), fetch(PC++));
        LD(nn);
        break;

      case 0xEB:
        break;

      case 0xEC:
        break;

      case 0xED:
        break;

      case 0xEE:
        n = make_u8(fetch(PC++));
        XOR(n);
        break;

      case 0xEF:
        RST(28);
        break;

      case 0xF0:
        n = make_u8(fetch(PC++));
        LD(n, Orientation::READ_FROM_IO_PORT);
        break;

      case 0xF1:

        POP(AF);

        A = AF.hi();
        F = AF.lo();
        break;

      case 0xF2:
        LD(Orientation::READ_FROM_IO_PORT);
        break;

      case 0xF3:
        DI();
        break;

      case 0xF4:
        break;

      case 0xF5:
        PUSH(AF);
        break;

      case 0xF6:
        n = make_u8(fetch(PC++));
        OR(n);
        break;

      case 0xF7:
        RST(30);
        break;

      case 0xF8:
        e = make_i8(fetch(PC++));
        LD(e);
        break;

      case 0xF9:
        LD();
        break;

      case 0xFA:
        nn = make_u16(fetch(PC++), fetch(PC++));
        LD(nn, /*dummy*/ 666);
        break;

      case 0xFB:
        EI();
        break;

      case 0xFC:
        break;

      case 0xFD:
        break;

      case 0xFE:
        n = make_u8(fetch(PC++));
        CP(n);
        break;

      case 0xFF:
        RST(38);
        break;
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
  r = bus->read(rr);

  c.tick(2);
}

void cpu::LD(const reg16 rr, const reg8 r) noexcept {
  bus->write(rr, r);

  c.tick(2);
}

void cpu::LDi() noexcept {
  bus->write(HL, A);
  HL += 1;

  c.tick(2);
}

void cpu::LDi([[maybe_unused]] int dummy) noexcept {
  A = bus->read(HL);
  HL += 1;

  c.tick(2);
}

void cpu::LDd() noexcept {
  HL -= 1;
  bus->write(HL, A);

  c.tick(2);
}

void cpu::LDd([[maybe_unused]] int dummy) noexcept {
  HL -= 1;
  A = bus->read(HL);

  c.tick(2);
}

void cpu::LD(const u16 nn) noexcept {
  bus->write(nn, A);
  c.tick(4);
}

void cpu::LD(const u16 nn, [[maybe_unused]] int dummy) noexcept {
  A = bus->read(nn);

  c.tick(4);
}

void cpu::LD(const u8 n, Orientation o) noexcept {
  switch (o) {
    case Orientation::READ_FROM_IO_PORT:
      A = bus->read(u16(0xFF00 + n));
      break;
    case Orientation::WRITE_TO_IO_PORT:
      bus->write(u16(0xFF00 + n), A);
      break;
  }

  c.tick(3);
}

void cpu::LD(Orientation o) noexcept {
  u16 C_reg_val = BC.lo();

  switch (o) {
    case Orientation::READ_FROM_IO_PORT:
      A = bus->read(u16(0xFF00 + C_reg_val));
      break;
    case Orientation::WRITE_TO_IO_PORT:
      bus->write(u16(0xFF00 + C_reg_val), A);
      break;
  }

  c.tick(2);
}

void cpu::LD(const reg16 rr, const u8 n, [[maybe_unused]] int dummy) noexcept {
  bus->write(rr, n);

  c.tick(3);
}

void cpu::LD(reg16 &rr, const u16 nn) noexcept {
  rr.lo(nn & 0x00FFU);
  rr.hi((nn & 0xFF00U) >> 8);

  c.tick(3);
}

void cpu::LD() noexcept {
  SP = HL;

  c.tick(2);
}

void cpu::LD(const i8 e) noexcept {
  HL = SP + e;

  c.tick(3);
}

void cpu::LD(const u16 nn, const reg16 rr) noexcept {
  //  m[nn] = rr.lo();
  bus->write(nn, rr.lo());
  bus->write(nn + 1, rr.hi());

  c.tick(5);
}
void cpu::PUSH(const reg16 rr) noexcept {
  bus->write(SP - 1, rr.lo());
  bus->write(SP - 2, rr.hi());

  SP -= 2;

  c.tick(4);
}

void cpu::POP(reg16 &rr) noexcept {
  rr.lo(bus->read(SP));
  rr.hi(bus->read(SP + 1));
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
  u8 n = bus->read(rr);
  ((n + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = bus->read(rr);
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

  u8 n = bus->read(rr);
  ((n + A.value()) == (reg8::max() + 1)) ? F.Z(set) : F.Z(reset);
  ((n + A.loNibble()) > 0b0000'1111) ? F.H(set) : F.H(reset);
  (n + A.value() > reg8::max()) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = bus->read(rr);

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

  u8 n = bus->read(rr);
  ((A.value() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.value() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = bus->read(rr);

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

  u8 n = bus->read(rr);
  ((A.value() - n) == reg8::min()) ? F.Z(set) : F.Z(reset);
  (A.loNibble() < n) ? F.H(set) : F.H(reset);
  (A.value() < n) ? F.C(set) : F.C(reset);

  reg8 tmp;
  tmp = bus->read(rr);

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

  (A.value() == 0 || bus->read(rr) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = bus->read(rr);

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

  (A.value() == 0 || bus->read(rr) == 0) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = bus->read(rr);

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

  (A.value() == bus->read(rr)) ? F.Z(set) : F.Z(reset);

  reg8 tmp;
  tmp = bus->read(rr);

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
  tmp = bus->read(rr);

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

  byte val = bus->read(HL);
  u8 lower_nibble = val | 0b0000'1111U;

  (lower_nibble == 0b0000'1111) ? F.H(set) : F.H(reset);
  (val == 0b1111'1111) ? F.Z(set) : F.Z(reset);

  bus->write(HL, byte(val + 1));
  c.tick(3);
}

void cpu::DEC(reg8 &r) noexcept {
  F.N(set);

  (r.loNibble() == 0) ? F.H(set) : F.H(reset);
  (r.value() == 1) ? F.Z(set) : F.Z(reset);

  r -= 1;
  c.tick(1);
}

void cpu::DEC() noexcept {
  F.N(set);

  byte val = bus->read(HL);
  u8 lower_nibble = val | 0b0000'1111U;

  (lower_nibble == 0) ? F.H(set) : F.H(reset);
  (val == 1) ? F.Z(set) : F.Z(reset);

  bus->write(HL, byte(val - 1));
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

void cpu::ADD(reg16 &rr, const i8 e) noexcept {
  F.N(reset);
  F.Z(reset);

  ((rr.value() + e) > reg16::max()) ? F.C(set) : F.C(reset);
  (((rr.value() & 0b0000'1111'1111'1111U) + e) > 4095U) ? F.H(set) : F.H(reset);

  rr += e;
  c.tick(4);
}

void cpu::INC(reg16 &rr) noexcept {
  rr += 1;
  c.tick(2);
}

void cpu::DEC(reg16 &rr) noexcept {
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

void cpu::RLC(reg8 &r) noexcept {
  F.H(reset);
  F.N(reset);

  r = rotl(r.value(), 1);
  (A.msb() == 1) ? F.C(set) : F.C(reset);
  (A.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::RLC(const reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  bus->write(rr, rotl(bus->read(rr), 1));

  reg8 tmp;
  tmp = bus->read(rr);

  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::RL(reg8 &r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;

  r = rotl(r.value(), 1);

  (r.msb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  r = r.value() | mask_for_0th_bit;

  c.tick(2);
}

void cpu::RL(const reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_0th_bit = F.C() ? 1 : 0;
  bus->write(rr, rotl(bus->read(rr), 1));

  reg8 tmp;
  tmp = bus->read(rr);

  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  bus->write(rr, byte(bus->read(rr) | mask_for_0th_bit));

  c.tick(4);
}

void cpu::RRC(reg8 &r) noexcept {
  F.H(reset);
  F.N(reset);

  r = rotr(r.value(), 1);
  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::RRC(const reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  bus->write(rr, rotr(bus->read(rr), 1));

  // create a temporary reg8 to reach individual bits of the byte
  // more easily, and set flags via it.
  reg8 tmp;
  tmp = bus->read(rr);
  (tmp.msb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == 0) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::RR(reg8 &r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  r = rotr(r.value(), 1);

  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  r = r.value() | mask_for_7th_bit;

  c.tick(2);
}

void cpu::RR(const reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  u8 mask_for_7th_bit = F.C() ? 0b1000'0000 : 0b0000'0000;

  bus->write(rr, rotr(bus->read(rr), 1));
  reg8 tmp;
  tmp = bus->read(rr);

  (tmp.lsb() == 1) ? F.C(set) : F.C(reset);
  (tmp.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  bus->write(rr, byte((bus->read(rr) | mask_for_7th_bit)));

  c.tick(4);
}

void cpu::SLA(reg8 &r) noexcept {
  F.H(reset);
  F.N(reset);

  (r.msb() == 1) ? F.C(set) : F.C(reset);
  r = r.value() << 1U;
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SLA(const reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  auto val = bus->read(rr);
  reg8 tmp;
  tmp = val;

  (tmp.msb() == 1) ? F.C(set) : F.C(reset);

  bus->write(rr, val <<= 1);

  (bus->read(rr) == 0U) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::SRA(reg8 &r) noexcept {
  F.H(reset);
  F.N(reset);

  u8 old_msb_mask = (r.msb() == 1) ? 0b1000'0000 : 0b0000'0000;

  r = r.value() >> 1U;
  r = r.value() | old_msb_mask;
  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  (r.value() == 0) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SRA(const reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  byte val = bus->read(rr);
  reg8 tmp;
  tmp = val;
  u8 old_msb_mask = (tmp.msb() == 1) ? 0b1000'0000 : 0b0000'0000;
  (tmp.lsb() == 1) ? F.C(set) : F.C(reset);

  bus->write(rr, val >>= 1);
  bus->write(rr, byte(bus->read(rr) | old_msb_mask));
  (bus->read(rr) == 0U) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::SRL(reg8 &r) noexcept {
  F.H(reset);
  F.N(reset);

  (r.lsb() == 1) ? F.C(set) : F.C(reset);
  r = r.value() >> 1;
  (r.value() == reg8::min()) ? F.Z(set) : F.Z(reset);

  c.tick(2);
}

void cpu::SRL(const reg16 rr) noexcept {
  F.H(reset);
  F.N(reset);

  byte val = bus->read(rr);
  reg8 tmp;
  tmp = val;
  (tmp.lsb() == 1) ? F.C(set) : F.C(reset);

  bus->write(rr, val >>= 1);
  (bus->read(rr) == 0U) ? F.Z(set) : F.Z(reset);

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

void cpu::SWAP(const reg16 rr) noexcept {
  F.C(reset);
  F.H(reset);
  F.N(reset);

  bus->write(rr, byte(rotl(bus->read(rr), 4)));

  (bus->read(rr) == 0U) ? F.Z(set) : F.Z(reset);

  c.tick(4);
}

void cpu::BIT(const reg8 r, const u8 pos) {
  F.H(set);
  F.N(reset);

  r.test(pos) ? F.Z(reset) : F.Z(set);

  c.tick(2);
}

void cpu::BIT(const reg16 rr, const u8 pos) {
  F.H(set);
  F.N(reset);

  reg8 tmp;
  tmp = bus->read(rr);

  tmp.test(pos) ? F.Z(reset) : F.Z(set);

  c.tick(3);
}

void cpu::SET(reg8 &r, const u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  // TODO: package this binary hack to
  // reg8 class, overload operator[] ?
  r = r.value() | (1U << pos);

  c.tick(2);
}

void cpu::SET(const reg16 rr, const u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  bus->write(rr, byte(bus->read(rr) | (1U << pos)));

  c.tick(4);
}

void cpu::RES(reg8 &r, const u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  r = r.value() & ~(1U << pos);

  c.tick(2);
}

void cpu::RES(const reg16 rr, const u8 pos) {
  if (pos < 0 || pos > 7) {
    throw std::out_of_range("reg8");
  }

  bus->write(rr, byte(bus->read(rr) & ~(1U << pos)));

  c.tick(4);
}

void cpu::JP(const u16 nn) noexcept {
  PC = nn;

  c.tick(4);
}

void cpu::JP(const u8 cc, const u16 nn) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    PC = nn;
    c.tick(4);
  } else {
    c.tick(3);
  }
}

void cpu::JP(const reg16 rr) noexcept {
  PC = rr;

  c.tick(1);
}

void cpu::JR(const i8 e) noexcept {
  PC += e;

  c.tick(3);
}

void cpu::JR(const u8 cc, const i8 e) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    PC += e;
    c.tick(3);
  } else {
    c.tick(2);
  }
}

void cpu::CALL(const u16 nn) noexcept {
  bus->write(SP - 1, PC.hi());
  bus->write(SP - 2, PC.lo());

  PC = nn;
  SP = SP - 2;

  c.tick(6);
}

void cpu::CALL(const u8 cc, const u16 nn) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    bus->write(SP - 1, PC.hi());
    bus->write(SP - 2, PC.lo());

    PC = nn;
    SP = SP - 2;

    c.tick(6);
  } else {
    c.tick(3);
  }
}

void cpu::RET() noexcept {
  PC.lo(bus->read(SP));
  PC.hi(bus->read(SP + 1));
  SP = SP + 2;

  c.tick(4);
}

void cpu::RETI() noexcept {
  PC.lo(bus->read(SP));
  PC.hi(bus->read(SP + 1));
  SP = SP + 2;

  IME = set;

  c.tick(4);
}

void cpu::RET(const u8 cc) noexcept {
  if ((cc == 0 && !F.Z()) || (cc == 1 && F.Z()) || (cc == 2 && !F.C()) || (cc == 3 && F.C())) {
    PC.lo(bus->read(SP));
    PC.hi(bus->read(SP + 1));
    SP = SP + 2;

    c.tick(5);
  } else {
    c.tick(2);
  }
}

void cpu::RST(const u8 t) noexcept {
  bus->write(SP - 1, PC.hi());
  bus->write(SP - 2, PC.lo());
  SP = SP - 2;

  PC.lo(0b0000'0000);

  if (t == 0) {
    PC.hi(0);
  } else if (t == 1) {
    PC.hi(8);
  } else if (t == 2) {
    PC.hi(10);
  } else if (t == 3) {
    PC.hi(18);
  } else if (t == 4) {
    PC.hi(20);
  } else if (t == 5) {
    PC.hi(28);
  } else if (t == 6) {
    PC.hi(30);
  } else if (t == 7) {
    PC.hi(38);
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

byte cpu::fetch(reg16 rr) { return bus->read(rr); }

}
