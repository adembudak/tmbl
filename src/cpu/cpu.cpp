#include "tmbl/config.h"
#include "tmbl/cpu/cpu.h"
#include "tmbl/bus/bus.h"
#include "tmbl/clock/clock.h"
#include "tmbl/cpu/internals/e8.h"
#include "tmbl/cpu/internals/flags.h"
#include "tmbl/cpu/internals/n8.h"
#include "tmbl/cpu/internals/r16.h"
#include "tmbl/cpu/internals/r8.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/io/registers.h"

#include <memory>

namespace tmbl {
cpu::cpu(std::shared_ptr<bus> pBus, std::shared_ptr<registers> pReg,
         std::shared_ptr<interrupts> pIntr, std::shared_ptr<clock> pClock)
    : m_pBus(pBus), m_pReg(pReg), m_pIntr(pIntr), m_pClock(pClock) {}

// see: double speed mode switch procedure
// https://archive.org/details/GameBoyProgManVer1.1/page/n35/mode/1up

void cpu::enableDoubleSpeedMode() {
  if ((KEY1 & 0b0100'0000) && (KEY1 & 0b0000'0001)) {
    m_pClock->enableDoubleSpeedMode(true);
    m_pReg->getAt(0xFF0F) = 0; // reset IF
    m_pIntr->IE = 0;
    m_pReg->getAt(0xFF00) |= 0x0011'0000;
    // STOP();
  }
}

enum class cpu::cc {
  Z,  // Execute if Z is set
  NZ, // Execute if Z is not set
  C,  // Execute if C is set
  NC  // Execute if C is not set
};

// register names adapted from: https://rgbds.gbdev.io/docs/master/gbz80.7
// opcode table adapted from: https://izik1.github.io/gbops/
void cpu::run() {

  // clang-format off
  for (;;) {
    switch (auto fetch = [&]{ return (m_pBus->readBus(PC++) | (m_pBus->readBus(PC++) << 8)); }; fetch()) {
        // clang-format on

      case 0x00:
        PC += 1;
        break;

      case 0x01:
        PC += 3;
        ld(BC, n16(fetch()));
        break;

      case 0x02:
        PC += 1;
        ld(BC.value(), A);
        break;

      case 0x03:
        PC += 1;
        inc(BC);
        break;

      case 0x04:
        PC += 1;
        inc(BC.hi());
        break;

      case 0x05:
        PC += 1;
        dec(BC.hi());
        break;

      case 0x06:
        PC += 2;
        ld(BC.hi(), n8(m_pBus->readBus(PC++)));
        break;

      case 0x07:
        PC += 1;
        rlca();
        break;

      case 0x08:
        PC += 3;
        break;

      case 0x09:
        PC += 1;
        add(BC);
        break;

      case 0x0A:
        PC += 1;
        ld(A, BC.value());
        break;

      case 0x0B:
        PC += 1;
        dec(BC);
        break;

      case 0x0C:
        PC += 1;
        inc(BC.lo());
        break;

      case 0x0D:
        PC += 1;
        add(BC.lo());
        break;

      case 0x0E:
        PC += 2;
        ld(BC.lo(), n8(m_pBus->readBus(PC++)));
        break;

      case 0x0F:
        PC += 1;
        rrca();
        break;

      case 0x10:
        PC += 1;
        dec(DE.hi());
        break;

      case 0x11:
        PC += 3;
        ld(DE, n16(fetch()));
        break;

      case 0x12:
        PC += 1;
        ld(DE.value(), A);
        break;

      case 0x13:
        PC += 1;
        inc(DE);
        break;

      case 0x14:
        PC += 1;
        inc(DE.hi());
        break;

      case 0x15:
        PC += 1;
        break;

      case 0x16:
        PC += 2;
        ld(DE.hi(), n8(m_pBus->readBus(PC++)));
        break;

      case 0x17:
        PC += 1;
        rla();
        break;

      case 0x18:
        PC += 2;
        jr(e8(m_pBus->readBus(PC++)));
        break;

      case 0x19:
        PC += 1;
        add(DE);
        break;

      case 0x1A:
        PC += 1;
        ld(A, DE.value());
        break;

      case 0x1B:
        PC += 1;
        dec(DE);
        break;

      case 0x1C:
        inc(DE.lo());
        PC += 1;
        break;

      case 0x1D:
        PC += 1;
        add(DE.lo());
        break;

      case 0x1E:
        PC += 2;
        ld(DE.lo(), n8(m_pBus->readBus(PC++)));
        break;

      case 0x1F:
        PC += 1;
        rra();
        break;

      case 0x20:
        PC += 2;
        jr(cc::NZ, e8(m_pBus->readBus(PC++)));
        break;

      case 0x21:
        PC += 3;
        ld(HL, n16(fetch()));
        break;

      case 0x22:
        PC += 1;
        ldi(HL, A);
        break;

      case 0x23:
        PC += 1;
        inc(HL);
        break;

      case 0x24:
        PC += 1;
        inc(HL.hi());
        break;

      case 0x25:
        PC += 1;
        dec(HL.hi());
        break;

      case 0x26:
        PC += 2;
        ld(HL.hi(), n8(m_pBus->readBus(PC++)));
        break;

      case 0x27:
        PC += 1;
        break;

      case 0x28:
        PC += 2;
        jr(cc::Z, e8(m_pBus->readBus(PC++)));
        break;

      case 0x29:
        PC += 1;
        add(HL);
        break;

      case 0x2A:
        PC += 1;
        ldi(A, HL);
        break;

      case 0x2B:
        PC += 1;
        dec(HL);
        break;

      case 0x2C:
        PC += 1;
        inc(HL.lo());
        break;

      case 0x2D:
        PC += 1;
        dec(HL.lo());
        break;

      case 0x2E:
        PC += 2;
        ld(HL.lo(), n8(m_pBus->readBus(PC++)));
        break;

      case 0x2F:
        PC += 1;
        cpl();
        break;

      case 0x30:
        PC += 2;
        jr(cc::NC, e8(m_pBus->readBus(PC++)));
        break;

      case 0x31:
        PC += 3;
        SP = n16(fetch()).value();
        m_clock.cycle(3);
        break;

      case 0x32:
        PC += 1;
        ldd(HL, A);
        break;

      case 0x33:
        PC += 1;
        ++SP;
        break;

      case 0x34:
        PC += 1;
        inc(HL.value());
        break;

      case 0x35:
        PC += 1;
        dec(HL.value());
        break;

      case 0x36:
        PC += 2;
        break;

      case 0x37:
        PC += 1;
        scf();
        break;

      case 0x38:
        PC += 2;
        jr(cc::C, e8(m_pBus->readBus(PC++)));
        break;

      case 0x39:
        PC += 1;
        //  add(SP); Fix this, failed overload resolution, calls add(byte b);
        break;

      case 0x3A:
        PC += 1;
        ldd(A, HL);
        break;

      case 0x3B:
        PC += 1;
        SP--;
        break;

      case 0x3C:
        PC += 1;
        inc(A);
        break;

      case 0x3D:
        PC += 1;
        dec(A);
        break;

      case 0x3E:
        PC += 2;
        ld(A, n8(m_pBus->readBus(PC++)));
        break;

      case 0x3F:
        PC += 1;
        ccf();
        break;

      case 0x40:
        PC += 1;
        ld(BC.hi(), BC.hi());
        break;

      case 0x41:
        PC += 1;
        ld(BC.hi(), BC.lo());
        break;

      case 0x42:
        PC += 1;
        ld(BC.hi(), DE.hi());
        break;

      case 0x43:
        PC += 1;
        ld(BC.hi(), DE.lo());
        break;

      case 0x44:
        PC += 1;
        ld(BC.hi(), HL.hi());
        break;

      case 0x45:
        PC += 1;
        ld(BC.hi(), HL.lo());
        break;

      case 0x46:
        PC += 1;
        ld(BC.hi(), HL.value());
        break;

      case 0x47:
        PC += 1;
        ld(BC.hi(), A);
        break;

      case 0x48:
        PC += 1;
        ld(BC.lo(), BC.hi());
        break;

      case 0x49:
        PC += 1;
        ld(BC.lo(), BC.lo());
        break;

      case 0x4A:
        PC += 1;
        ld(BC.lo(), DE.hi());
        break;

      case 0x4B:
        PC += 1;
        ld(BC.lo(), DE.lo());
        break;

      case 0x4C:
        PC += 1;
        ld(BC.lo(), HL.hi());
        break;

      case 0x4D:
        PC += 1;
        ld(BC.lo(), HL.lo());
        break;

      case 0x4E:
        PC += 1;
        ld(BC.lo(), HL.value());
        break;

      case 0x4F:
        PC += 1;
        ld(BC.lo(), A);
        break;

      case 0x50:
        PC += 1;
        ld(DE.hi(), BC.hi());
        break;

      case 0x51:
        PC += 1;
        ld(DE.hi(), BC.lo());
        break;

      case 0x52:
        PC += 1;
        ld(DE.hi(), DE.hi());
        break;

      case 0x53:
        PC += 1;
        ld(DE.hi(), DE.lo());
        break;

      case 0x54:
        PC += 1;
        ld(DE.hi(), HL.hi());
        break;

      case 0x55:
        PC += 1;
        ld(DE.hi(), HL.lo());
        break;

      case 0x56:
        PC += 1;
        ld(DE.hi(), HL.value());
        break;

      case 0x57:
        PC += 1;
        ld(DE.hi(), A);
        break;

      case 0x58:
        PC += 1;
        ld(DE.lo(), BC.hi());
        break;

      case 0x59:
        PC += 1;
        ld(DE.lo(), BC.lo());
        break;

      case 0x5A:
        PC += 1;
        ld(DE.lo(), DE.hi());
        break;

      case 0x5B:
        PC += 1;
        ld(DE.lo(), DE.lo());
        break;

      case 0x5C:
        PC += 1;
        ld(DE.lo(), HL.hi());
        break;

      case 0x5D:
        PC += 1;
        ld(DE.lo(), HL.lo());
        break;

      case 0x5E:
        PC += 1;
        ld(DE.lo(), HL.value());
        break;

      case 0x5F:
        PC += 1;
        ld(DE.lo(), A);
        break;

      case 0x60:
        PC += 1;
        ld(HL.hi(), BC.hi());
        break;

      case 0x61:
        PC += 1;
        ld(HL.hi(), BC.lo());
        break;

      case 0x62:
        PC += 1;
        ld(HL.hi(), DE.hi());
        break;

      case 0x63:
        PC += 1;
        ld(HL.hi(), DE.lo());
        break;

      case 0x64:
        PC += 1;
        ld(HL.hi(), HL.hi());
        break;

      case 0x65:
        PC += 1;
        ld(HL.hi(), HL.lo());
        break;

      case 0x66:
        PC += 1;
        ld(HL.hi(), HL.value());
        break;

      case 0x67:
        PC += 1;
        ld(HL.hi(), A);
        break;

      case 0x68:
        PC += 1;
        ld(HL.lo(), BC.hi());
        break;

      case 0x69:
        PC += 1;
        ld(HL.lo(), BC.lo());
        break;

      case 0x6A:
        PC += 1;
        ld(HL.lo(), DE.hi());
        break;

      case 0x6B:
        PC += 1;
        ld(HL.lo(), DE.lo());
        break;

      case 0x6C:
        PC += 1;
        ld(HL.lo(), HL.hi());
        break;

      case 0x6D:
        PC += 1;
        ld(HL.lo(), HL.lo());
        break;

      case 0x6E:
        PC += 1;
        ld(HL.lo(), HL.value());
        break;

      case 0x6F:
        PC += 1;
        ld(HL.lo(), A);
        break;

      case 0x70:
        PC += 1;
        ld(HL.value(), BC.hi());
        break;

      case 0x71:
        PC += 1;
        ld(HL.value(), BC.lo());
        break;

      case 0x72:
        PC += 1;
        ld(HL.value(), DE.hi());
        break;

      case 0x73:
        PC += 1;
        ld(HL.value(), DE.lo());
        break;

      case 0x74:
        PC += 1;
        ld(HL.value(), HL.hi());
        break;

      case 0x75:
        PC += 1;
        ld(HL.value(), HL.lo());
        break;

      case 0x76:
        PC += 1;
        break;

      case 0x77:
        PC += 1;
        ld(HL.value(), A);
        break;

      case 0x78:
        PC += 1;
        ld(A, BC.hi());
        break;

      case 0x79:
        PC += 1;
        ld(A, BC.lo());
        break;

      case 0x7A:
        PC += 1;
        ld(A, DE.hi());
        break;

      case 0x7B:
        PC += 1;
        ld(A, DE.lo());
        break;

      case 0x7C:
        PC += 1;
        ld(A, HL.hi());
        break;

      case 0x7D:
        PC += 1;
        ld(A, HL.lo());
        break;

      case 0x7E:
        PC += 1;
        ld(A, HL.value());
        break;

      case 0x7F:
        PC += 1;
        ld(A, A);
        break;

      case 0x80:
        PC += 1;
        add(BC.hi());
        break;

      case 0x81:
        PC += 1;
        add(BC.lo());
        break;

      case 0x82:
        PC += 1;
        add(DE.hi());
        break;

      case 0x83:
        PC += 1;
        add(DE.lo());
        break;

      case 0x84:
        PC += 1;
        add(HL.hi());
        break;

      case 0x85:
        PC += 1;
        add(HL.lo());
        break;

      case 0x86:
        PC += 1;
        add(m_pBus->readBus(HL.value()));
        break;

      case 0x87:
        PC += 1;
        add(A);
        break;

      case 0x88:
        PC += 1;
        adc(BC.hi());
        break;

      case 0x89:
        PC += 1;
        adc(BC.lo());
        break;

      case 0x8A:
        PC += 1;
        adc(DE.hi());
        break;

      case 0x8B:
        PC += 1;
        adc(DE.lo());
        break;

      case 0x8C:
        PC += 1;
        adc(HL.hi());
        break;

      case 0x8D:
        PC += 1;
        adc(HL.lo());
        break;

      case 0x8E:
        PC += 1;
        adc(m_pBus->readBus(HL.value()));
        break;

      case 0x8F:
        PC += 1;
        adc(A);
        break;

      case 0x90:
        PC += 1;
        sub(BC.hi());
        break;

      case 0x91:
        PC += 1;
        sub(BC.lo());
        break;

      case 0x92:
        PC += 1;
        sub(DE.hi());
        break;

      case 0x93:
        PC += 1;
        sub(DE.lo());
        break;

      case 0x94:
        PC += 1;
        sub(HL.hi());
        break;

      case 0x95:
        PC += 1;
        sub(HL.lo());
        break;

      case 0x96:
        PC += 1;
        sub(m_pBus->readBus(HL.value()));
        break;
        break;

      case 0x97:
        PC += 1;
        sub(A);
        break;

      case 0x98:
        PC += 1;
        sbc(BC.hi());
        break;

      case 0x99:
        PC += 1;
        sbc(BC.lo());
        break;

      case 0x9A:
        PC += 1;
        sbc(DE.hi());
        break;

      case 0x9B:
        PC += 1;
        sbc(DE.lo());
        break;

      case 0x9C:
        PC += 1;
        sbc(HL.hi());
        break;

      case 0x9D:
        PC += 1;
        sbc(HL.lo());
        break;

      case 0x9E:
        PC += 1;
        sbc(m_pBus->readBus(HL.value()));
        break;

      case 0x9F:
        PC += 1;
        sbc(A);
        break;

      case 0xA0:
        PC += 1;
        and_(BC.hi());
        break;

      case 0xA1:
        PC += 1;
        and_(BC.lo());
        break;

      case 0xA2:
        PC += 1;
        and_(DE.hi());
        break;

      case 0xA3:
        PC += 1;
        and_(DE.hi());
        break;

      case 0xA4:
        PC += 1;
        and_(HL.hi());
        break;

      case 0xA5:
        PC += 1;
        and_(HL.lo());
        break;

      case 0xA6:
        PC += 1;
        and_(m_pBus->readBus(HL.value()));
        break;

      case 0xA7:
        PC += 1;
        and_(A);
        break;

      case 0xA8:
        PC += 1;
        xor_(BC.hi());
        break;

      case 0xA9:
        PC += 1;
        xor_(BC.lo());
        break;

      case 0xAA:
        PC += 1;
        xor_(DE.hi());
        break;

      case 0xAB:
        PC += 1;
        xor_(DE.lo());
        break;

      case 0xAC:
        PC += 1;
        xor_(HL.hi());
        break;

      case 0xAD:
        PC += 1;
        xor_(HL.lo());
        break;

      case 0xAE:
        PC += 1;
        xor_(m_pBus->readBus(HL.value()));
        break;

      case 0xAF:
        PC += 1;
        xor_(A);
        break;

      case 0xB0:
        PC += 1;
        or_(BC.hi());
        break;

      case 0xB1:
        PC += 1;
        or_(BC.lo());
        break;

      case 0xB2:
        PC += 1;
        or_(DE.hi());
        break;

      case 0xB3:
        PC += 1;
        or_(DE.lo());
        break;

      case 0xB4:
        PC += 1;
        or_(HL.hi());
        break;

      case 0xB5:
        PC += 1;
        or_(HL.lo());
        break;

      case 0xB6:
        PC += 1;
        or_(m_pBus->readBus(HL.value()));
        break;

      case 0xB7:
        PC += 1;
        or_(A);
        break;

      case 0xB8:
        PC += 1;
        cp(BC.hi());
        break;

      case 0xB9:
        PC += 1;
        cp(BC.lo());
        break;

      case 0xBA:
        PC += 1;
        cp(DE.hi());
        break;

      case 0xBB:
        PC += 1;
        cp(DE.lo());
        break;

      case 0xBC:
        PC += 1;
        cp(HL.hi());
        break;

      case 0xBD:
        PC += 1;
        cp(HL.lo());
        break;

      case 0xBE:
        PC += 1;
        cp(m_pBus->readBus(HL.value()));
        break;

      case 0xBF:
        PC += 1;
        cp(A);
        break;

      case 0xC0:
        PC += 1;
        ret(cc::NZ);
        break;

      case 0xC1:
        PC += 1;
        pop(BC);
        break;

      case 0xC2:
        PC += 3;
        jp(cc::NZ, n16(m_pBus->readBus(PC++)));
        break;

      case 0xC3:
        PC += 3;
        jp(n16(m_pBus->readBus(PC++)));
        break;

      case 0xC4:
        PC += 3;
        call(cc::NZ, n16(m_pBus->readBus(PC++)));
        break;

      case 0xC5:
        PC += 1;
        push(BC);
        break;

      case 0xC6:
        PC += 2;
        add(m_pBus->readBus(PC++));
        break;

      case 0xC7:
        PC += 1;
        rst(0);
        break;

      case 0xC8:
        PC += 1;
        ret(cc::Z);
        break;

      case 0xC9:
        PC += 1;
        ret();
        break;

      case 0xCA:
        PC += 3;
        jp(cc::Z, n16(m_pBus->readBus(PC++)));
        break;

      case 0xCB: // 0xCB Prefixed:
        PC += 1;
        switch (fetch()) {
          case 0x00:
            PC += 2;
            rlc(BC.hi());
            break;

          case 0x01:
            PC += 2;
            rlc(BC.lo());
            break;

          case 0x02:
            PC += 2;
            rlc(DE.hi());
            break;

          case 0x03:
            PC += 2;
            rlc(DE.lo());
            break;

          case 0x04:
            PC += 2;
            rlc(HL.hi());
            break;

          case 0x05:
            PC += 2;
            rlc(HL.lo());
            break;

          case 0x06:
            PC += 2;
            rlc(HL.value());
            break;

          case 0x07:
            PC += 2;
            rlc(A);
            break;

          case 0x08:
            PC += 2;
            rrc(BC.hi());
            break;

          case 0x09:
            PC += 2;
            rrc(BC.lo());
            break;

          case 0x0A:
            PC += 2;
            rrc(DE.hi());
            break;

          case 0x0B:
            PC += 2;
            rrc(DE.lo());
            break;

          case 0x0C:
            PC += 2;
            rrc(HL.hi());
            break;

          case 0x0D:
            PC += 2;
            rrc(HL.lo());
            break;

          case 0x0E:
            PC += 2;
            rrc(HL.value());
            break;

          case 0x0F:
            PC += 2;
            rrc(A);
            break;

          case 0x10:
            PC += 2;
            rl(BC.hi());
            break;

          case 0x11:
            PC += 2;
            rl(BC.lo());
            break;

          case 0x12:
            PC += 2;
            rl(DE.hi());
            break;

          case 0x13:
            PC += 2;
            rl(DE.lo());
            break;

          case 0x14:
            PC += 2;
            rl(HL.hi());
            break;

          case 0x15:
            PC += 2;
            rl(HL.lo());
            break;

          case 0x16:
            PC += 2;
            rl(HL.value());
            break;

          case 0x17:
            PC += 2;
            rl(A);
            break;

          case 0x18:
            PC += 2;
            rr(BC.hi());
            break;

          case 0x19:
            PC += 2;
            rr(BC.lo());
            break;

          case 0x1A:
            PC += 2;
            rr(DE.hi());
            break;

          case 0x1B:
            PC += 2;
            rr(DE.lo());
            break;

          case 0x1C:
            PC += 2;
            rr(HL.hi());
            break;

          case 0x1D:
            PC += 2;
            rr(HL.lo());
            break;

          case 0x1E:
            PC += 2;
            rr(HL.value());
            break;

          case 0x1F:
            PC += 2;
            rr(A);
            break;

          case 0x20:
            PC += 2;
            sla(BC.hi());
            break;

          case 0x21:
            PC += 2;
            sla(BC.lo());
            break;

          case 0x22:
            PC += 2;
            sla(DE.hi());
            break;

          case 0x23:
            PC += 2;
            sla(DE.lo());
            break;

          case 0x24:
            PC += 2;
            sla(HL.hi());
            break;

          case 0x25:
            PC += 2;
            sla(HL.lo());
            break;

          case 0x26:
            PC += 2;
            sla(HL.value());
            break;

          case 0x27:
            PC += 2;
            sla(A);
            break;

          case 0x28:
            PC += 2;
            sra(BC.hi());
            break;

          case 0x29:
            PC += 2;
            sra(BC.lo());
            break;

          case 0x2A:
            PC += 2;
            sra(DE.hi());
            break;

          case 0x2B:
            PC += 2;
            sra(DE.lo());
            break;

          case 0x2C:
            PC += 2;
            sra(HL.hi());
            break;

          case 0x2D:
            PC += 2;
            sra(HL.lo());
            break;

          case 0x2E:
            PC += 2;
            sra(HL.value());
            break;

          case 0x2F:
            PC += 2;
            sra(A);
            break;

          case 0x30:
            PC += 2;
            swap(BC.hi());
            break;

          case 0x31:
            PC += 2;
            swap(BC.lo());
            break;

          case 0x32:
            PC += 2;
            swap(DE.hi());
            break;

          case 0x33:
            PC += 2;
            swap(DE.lo());
            break;

          case 0x34:
            PC += 2;
            swap(HL.hi());
            break;

          case 0x35:
            PC += 2;
            swap(HL.lo());
            break;

          case 0x36:
            PC += 2;
            swap(HL.value());
            break;

          case 0x37:
            PC += 2;
            swap(A);
            break;

          case 0x38:
            PC += 2;
            srl(BC.hi());
            break;

          case 0x39:
            PC += 2;
            srl(BC.lo());
            break;

          case 0x3A:
            PC += 2;
            srl(DE.hi());
            break;

          case 0x3B:
            PC += 2;
            srl(DE.lo());
            break;

          case 0x3C:
            PC += 2;
            srl(HL.hi());
            break;

          case 0x3D:
            PC += 2;
            srl(HL.lo());
            break;

          case 0x3E:
            PC += 2;
            srl(HL.value());
            break;

          case 0x3F:
            PC += 2;
            srl(A);
            break;

          case 0x40:
            PC += 2;
            bit(0, BC.hi());
            break;

          case 0x41:
            PC += 2;
            bit(0, BC.lo());
            break;

          case 0x42:
            PC += 2;
            bit(0, DE.hi());
            break;

          case 0x43:
            PC += 2;
            bit(0, DE.lo());
            break;

          case 0x44:
            PC += 2;
            bit(0, HL.hi());
            break;

          case 0x45:
            PC += 2;
            bit(0, HL.lo());
            break;

          case 0x46:
            PC += 2;
            bit(0, m_pBus->readBus(HL.value()));
            break;

          case 0x47:
            PC += 2;
            bit(0, A);
            break;

          case 0x48:
            PC += 2;
            bit(1, BC.hi());
            break;

          case 0x49:
            PC += 2;
            bit(1, BC.lo());
            break;

          case 0x4A:
            PC += 2;
            bit(1, DE.hi());
            break;

          case 0x4B:
            PC += 2;
            bit(1, DE.lo());
            break;

          case 0x4C:
            PC += 2;
            bit(1, HL.hi());
            break;

          case 0x4D:
            PC += 2;
            bit(1, HL.lo());
            break;

          case 0x4E:
            PC += 2;
            bit(1, m_pBus->readBus(HL.value()));
            break;

          case 0x4F:
            PC += 2;
            bit(1, A);
            break;

          case 0x50:
            PC += 2;
            bit(2, BC.hi());
            break;

          case 0x51:
            PC += 2;
            bit(2, BC.lo());
            break;

          case 0x52:
            PC += 2;
            bit(2, DE.hi());
            break;

          case 0x53:
            PC += 2;
            bit(2, DE.lo());
            break;

          case 0x54:
            PC += 2;
            bit(2, HL.hi());
            break;

          case 0x55:
            PC += 2;
            bit(2, HL.lo());
            break;

          case 0x56:
            PC += 2;
            bit(2, m_pBus->readBus(HL.value()));
            break;

          case 0x57:
            PC += 2;
            bit(2, A);
            break;

          case 0x58:
            PC += 2;
            bit(3, BC.hi());
            break;

          case 0x59:
            PC += 2;
            bit(3, BC.lo());
            break;

          case 0x5A:
            PC += 2;
            bit(3, DE.hi());
            break;

          case 0x5B:
            PC += 2;
            bit(3, DE.lo());
            break;

          case 0x5C:
            PC += 2;
            bit(3, HL.hi());
            break;

          case 0x5D:
            PC += 2;
            bit(3, HL.lo());
            break;

          case 0x5E:
            PC += 2;
            bit(3, m_pBus->readBus(HL.value()));
            break;

          case 0x5F:
            PC += 2;
            bit(3, A);
            break;

          case 0x60:
            PC += 2;
            bit(4, BC.hi());
            break;

          case 0x61:
            PC += 2;
            bit(4, BC.lo());
            break;

          case 0x62:
            PC += 2;
            bit(4, DE.hi());
            break;

          case 0x63:
            PC += 2;
            bit(4, DE.lo());
            break;

          case 0x64:
            PC += 2;
            bit(4, HL.hi());
            break;

          case 0x65:
            PC += 2;
            bit(4, HL.lo());
            break;

          case 0x66:
            PC += 2;
            bit(4, m_pBus->readBus(HL.value()));
            break;

          case 0x67:
            PC += 2;
            bit(4, A);
            break;

          case 0x68:
            PC += 2;
            bit(5, BC.hi());
            break;

          case 0x69:
            PC += 2;
            bit(5, BC.lo());
            break;

          case 0x6A:
            PC += 2;
            bit(5, DE.hi());
            break;

          case 0x6B:
            PC += 2;
            bit(5, DE.lo());
            break;

          case 0x6C:
            PC += 2;
            bit(5, HL.hi());
            break;

          case 0x6D:
            PC += 2;
            bit(5, HL.lo());
            break;

          case 0x6E:
            PC += 2;
            bit(5, m_pBus->readBus(HL.value()));
            break;

          case 0x6F:
            PC += 2;
            bit(5, A);
            break;

          case 0x70:
            PC += 2;
            bit(6, BC.hi());
            break;

          case 0x71:
            PC += 2;
            bit(6, BC.lo());
            break;

          case 0x72:
            PC += 2;
            bit(6, DE.hi());
            break;

          case 0x73:
            PC += 2;
            bit(6, DE.lo());
            break;

          case 0x74:
            PC += 2;
            bit(6, HL.hi());
            break;

          case 0x75:
            PC += 2;
            bit(6, HL.lo());
            break;

          case 0x76:
            PC += 2;
            bit(6, m_pBus->readBus(HL.value()));
            break;

          case 0x77:
            PC += 2;
            bit(6, A);
            break;

          case 0x78:
            PC += 2;
            bit(7, BC.hi());
            break;

          case 0x79:
            PC += 2;
            bit(7, BC.lo());
            break;

          case 0x7A:
            PC += 2;
            bit(7, DE.hi());
            break;

          case 0x7B:
            PC += 2;
            bit(7, DE.lo());
            break;

          case 0x7C:
            PC += 2;
            bit(7, HL.hi());
            break;

          case 0x7D:
            PC += 2;
            bit(7, HL.lo());
            break;

          case 0x7E:
            PC += 2;
            bit(7, m_pBus->readBus(HL.value()));
            break;

          case 0x7F:
            PC += 2;
            bit(7, A);
            break;

          case 0x80:
            PC += 2;
            res(0, BC.hi());
            break;

          case 0x81:
            PC += 2;
            res(0, BC.lo());
            break;

          case 0x82:
            PC += 2;
            res(0, DE.hi());
            break;

          case 0x83:
            PC += 2;
            res(0, DE.lo());
            break;

          case 0x84:
            PC += 2;
            res(0, HL.hi());
            break;

          case 0x85:
            PC += 2;
            res(0, HL.lo());
            break;

          case 0x86:
            PC += 2;
            res(0, HL.value());
            break;

          case 0x87:
            PC += 2;
            res(0, A);
            break;

          case 0x88:
            PC += 2;
            res(1, BC.hi());
            break;

          case 0x89:
            PC += 2;
            res(1, BC.lo());
            break;

          case 0x8A:
            PC += 2;
            res(1, DE.hi());
            break;

          case 0x8B:
            PC += 2;
            res(1, DE.lo());
            break;

          case 0x8C:
            PC += 2;
            res(1, HL.hi());
            break;

          case 0x8D:
            PC += 2;
            res(1, HL.lo());
            break;

          case 0x8E:
            PC += 2;
            res(1, HL.value());
            break;

          case 0x8F:
            PC += 2;
            res(1, A);
            break;

          case 0x90:
            PC += 2;
            res(2, BC.hi());
            break;

          case 0x91:
            PC += 2;
            res(2, BC.lo());
            break;

          case 0x92:
            PC += 2;
            res(2, DE.hi());
            break;

          case 0x93:
            PC += 2;
            res(2, DE.lo());
            break;

          case 0x94:
            PC += 2;
            res(2, HL.hi());
            break;

          case 0x95:
            PC += 2;
            res(2, HL.lo());
            break;

          case 0x96:
            PC += 2;
            res(2, HL.value());
            break;

          case 0x97:
            PC += 2;
            res(2, A);
            break;

          case 0x98:
            PC += 2;
            res(3, BC.hi());
            break;

          case 0x99:
            PC += 2;
            res(3, BC.lo());
            break;

          case 0x9A:
            PC += 2;
            res(3, DE.hi());
            break;

          case 0x9B:
            PC += 2;
            res(3, DE.lo());
            break;

          case 0x9C:
            PC += 2;
            res(3, HL.hi());
            break;

          case 0x9D:
            PC += 2;
            res(3, HL.lo());
            break;

          case 0x9E:
            PC += 2;
            res(3, HL.value());
            break;

          case 0x9F:
            PC += 2;
            res(3, A);
            break;

          case 0xA0:
            PC += 2;
            res(4, BC.hi());
            break;

          case 0xA1:
            PC += 2;
            res(4, BC.lo());
            break;

          case 0xA2:
            PC += 2;
            res(4, DE.hi());
            break;

          case 0xA3:
            PC += 2;
            res(4, DE.lo());
            break;

          case 0xA4:
            PC += 2;
            res(4, HL.hi());
            break;

          case 0xA5:
            PC += 2;
            res(4, HL.lo());
            break;

          case 0xA6:
            PC += 2;
            res(4, HL.value());
            break;

          case 0xA7:
            PC += 2;
            res(4, A);
            break;

          case 0xA8:
            PC += 2;
            res(5, BC.hi());
            break;

          case 0xA9:
            PC += 2;
            res(5, BC.lo());
            break;

          case 0xAA:
            PC += 2;
            res(5, DE.hi());
            break;

          case 0xAB:
            PC += 2;
            res(5, DE.lo());
            break;

          case 0xAC:
            PC += 2;
            res(5, HL.hi());
            break;

          case 0xAD:
            PC += 2;
            res(5, HL.lo());
            break;

          case 0xAE:
            PC += 2;
            res(5, HL.value());
            break;

          case 0xAF:
            PC += 2;
            res(5, A);
            break;

          case 0xB0:
            PC += 2;
            res(6, BC.hi());
            break;

          case 0xB1:
            PC += 2;
            res(6, BC.lo());
            break;

          case 0xB2:
            PC += 2;
            res(6, DE.hi());
            break;

          case 0xB3:
            PC += 2;
            res(6, DE.lo());
            break;

          case 0xB4:
            PC += 2;
            res(6, HL.hi());
            break;

          case 0xB5:
            PC += 2;
            res(6, HL.lo());
            break;

          case 0xB6:
            PC += 2;
            res(6, HL.value());
            break;

          case 0xB7:
            PC += 2;
            res(6, A);
            break;

          case 0xB8:
            PC += 2;
            res(7, BC.hi());
            break;

          case 0xB9:
            PC += 2;
            res(7, BC.lo());
            break;

          case 0xBA:
            PC += 2;
            res(7, DE.hi());
            break;

          case 0xBB:
            PC += 2;
            res(7, DE.lo());
            break;

          case 0xBC:
            PC += 2;
            res(7, HL.hi());
            break;

          case 0xBD:
            PC += 2;
            res(7, HL.lo());
            break;

          case 0xBE:
            PC += 2;
            res(7, HL.value());
            break;

          case 0xBF:
            PC += 2;
            res(7, A);
            break;

          case 0xC0:
            set_(0, BC.hi());
            PC += 2;
            break;

          case 0xC1:
            PC += 2;
            set_(0, BC.lo());
            break;

          case 0xC2:
            PC += 2;
            set_(0, DE.hi());
            break;

          case 0xC3:
            PC += 2;
            set_(0, DE.lo());
            break;

          case 0xC4:
            PC += 2;
            set_(0, HL.hi());
            break;

          case 0xC5:
            PC += 2;
            set_(0, HL.lo());
            break;

          case 0xC6:
            PC += 2;
            set_(0, HL.value());
            break;

          case 0xC7:
            PC += 2;
            set_(0, A);
            break;

          case 0xC8:
            PC += 2;
            set_(1, BC.hi());
            break;

          case 0xC9:
            PC += 2;
            set_(1, BC.lo());
            break;

          case 0xCA:
            PC += 2;
            set_(1, DE.hi());
            break;

          case 0xCB:
            PC += 2;
            set_(1, DE.lo());
            break;

          case 0xCC:
            PC += 2;
            set_(1, HL.hi());
            break;

          case 0xCD:
            PC += 2;
            set_(1, HL.lo());
            break;

          case 0xCE:
            PC += 2;
            set_(1, HL.value());
            break;

          case 0xCF:
            PC += 2;
            set_(1, A);
            break;

          case 0xD0:
            PC += 2;
            set_(2, BC.hi());
            break;

          case 0xD1:
            PC += 2;
            set_(2, BC.lo());
            break;

          case 0xD2:
            PC += 2;
            set_(2, DE.hi());
            break;

          case 0xD3:
            PC += 2;
            set_(2, DE.lo());
            break;

          case 0xD4:
            PC += 2;
            set_(2, HL.hi());
            break;

          case 0xD5:
            PC += 2;
            set_(2, HL.lo());
            break;

          case 0xD6:
            PC += 2;
            set_(2, HL.value());
            break;

          case 0xD7:
            PC += 2;
            set_(2, A);
            break;

          case 0xD8:
            PC += 2;
            set_(3, BC.hi());
            break;

          case 0xD9:
            PC += 2;
            set_(3, BC.lo());
            break;

          case 0xDA:
            PC += 2;
            set_(3, DE.hi());
            break;

          case 0xDB:
            PC += 2;
            set_(3, DE.lo());
            break;

          case 0xDC:
            PC += 2;
            set_(3, HL.hi());
            break;

          case 0xDD:
            PC += 2;
            set_(3, HL.lo());
            break;

          case 0xDE:
            PC += 2;
            set_(3, HL.value());
            break;

          case 0xDF:
            PC += 2;
            set_(3, A);
            break;

          case 0xE0:
            PC += 2;
            set_(4, BC.hi());
            break;

          case 0xE1:
            PC += 2;
            set_(4, BC.lo());
            break;

          case 0xE2:
            PC += 2;
            set_(4, DE.hi());
            break;

          case 0xE3:
            PC += 2;
            set_(4, DE.lo());
            break;

          case 0xE4:
            PC += 2;
            set_(4, HL.hi());
            break;

          case 0xE5:
            PC += 2;
            set_(4, HL.lo());
            break;

          case 0xE6:
            PC += 2;
            set_(4, HL.value());
            break;

          case 0xE7:
            PC += 2;
            set_(4, A);
            break;

          case 0xE8:
            PC += 2;
            set_(5, BC.hi());
            break;

          case 0xE9:
            PC += 2;
            set_(5, BC.lo());
            break;

          case 0xEA:
            PC += 2;
            set_(5, DE.hi());
            break;

          case 0xEB:
            PC += 2;
            set_(5, DE.lo());
            break;

          case 0xEC:
            PC += 2;
            set_(5, HL.hi());
            break;

          case 0xED:
            PC += 2;
            set_(5, HL.lo());
            break;

          case 0xEE:
            PC += 2;
            set_(5, HL.value());
            break;

          case 0xEF:
            PC += 2;
            set_(5, A);
            break;

          case 0xF0:
            PC += 2;
            set_(6, BC.hi());
            break;

          case 0xF1:
            PC += 2;
            set_(6, BC.lo());
            break;

          case 0xF2:
            PC += 2;
            set_(6, DE.hi());
            break;

          case 0xF3:
            PC += 2;
            set_(6, DE.lo());
            break;

          case 0xF4:
            PC += 2;
            set_(6, HL.hi());
            break;

          case 0xF5:
            PC += 2;
            set_(6, HL.lo());
            break;

          case 0xF6:
            PC += 2;
            set_(6, HL.value());
            break;

          case 0xF7:
            PC += 2;
            set_(6, A);
            break;

          case 0xF8:
            PC += 2;
            set_(7, BC.hi());
            break;

          case 0xF9:
            PC += 2;
            set_(7, BC.lo());
            break;

          case 0xFA:
            PC += 2;
            set_(7, DE.hi());
            break;

          case 0xFB:
            PC += 2;
            set_(7, DE.lo());
            break;

          case 0xFC:
            PC += 2;
            set_(7, HL.hi());
            break;

          case 0xFD:
            PC += 2;
            set_(7, HL.lo());
            break;

          case 0xFE:
            PC += 2;
            set_(7, HL.value());
            break;

          case 0xFF:
            PC += 2;
            set_(7, A);
            break;
        }
        break;

      case 0xCC:
        PC += 3;
        call(cc::Z, n16(m_pBus->readBus(PC++)));
        break;

      case 0xCD:
        PC += 3;
        call(n16(m_pBus->readBus(PC++)));
        break;

      case 0xCE:
        PC += 2;
        adc(n8(m_pBus->readBus(PC++)));
        break;

      case 0xCF:
        PC += 1;
        rst(1);
        break;

      case 0xD0:
        PC += 1;
        ret(cc::NC);
        break;

      case 0xD1:
        PC += 1;
        pop(DE);
        break;

      case 0xD2:
        PC += 3;
        jp(cc::NC, n16(m_pBus->readBus(PC++)));
        break;

      case 0xD4:
        PC += 3;
        call(cc::NC, n16(m_pBus->readBus(PC++)));
        break;

      case 0xD5:
        PC += 1;
        push(DE);
        break;

      case 0xD6:
        PC += 2;
        sub(n8(m_pBus->readBus(PC++)));
        break;

      case 0xD7:
        PC += 1;
        rst(2);
        break;

      case 0xD8:
        PC += 1;
        ret(cc::C);
        break;

      case 0xD9:
        PC += 1;
        reti();
        break;

      case 0xDA:
        PC += 3;
        jp(cc::C, n16(m_pBus->readBus(PC++)));
        break;

      case 0xDC:
        PC += 3;
        call(cc::C, n16(m_pBus->readBus(PC++)));
        break;

      case 0xDE:
        PC += 2;
        sbc(n8(m_pBus->readBus(PC++)));
        break;

      case 0xDF:
        PC += 1;
        rst(3);
        break;

      case 0xE0:
        PC += 2;
        ldio(0xFF00 + n8(m_pBus->readBus(PC++)).value(), A);
        break;

      case 0xE1:
        PC += 1;
        pop(HL);
        break;

      case 0xE2:
        PC += 1;
        ldio(0xFF00 + BC.lo().value(), A);
        break;

      case 0xE5:
        PC += 1;
        push(HL);
        break;

      case 0xE6:
        PC += 2;
        and_(n8(m_pBus->readBus(PC++)));
        break;

      case 0xE7:
        PC += 1;
        rst(4);
        break;

      case 0xE8:
        PC += 2;
        add(e8(m_pBus->readBus(PC++)));
        break;

      case 0xE9:
        PC += 1;
        jp();
        break;

      case 0xEA:
        PC += 3;
        break;

      case 0xEE:
        PC += 2;
        xor_(n8(m_pBus->readBus(PC++)));
        break;

      case 0xEF:
        PC += 1;
        rst(5);
        break;

      case 0xF0:
        PC += 2;
        ldio(A, 0xFF00 + n8(m_pBus->readBus(PC++)).value());
        break;

      case 0xF1:
        PC += 1;
        pop();
        break;

      case 0xF2:
        PC += 1;
        ldio(A, 0xFF00 + BC.lo().value());
        break;

      case 0xF3:
        PC += 1;
        di();
        break;

      case 0xF5:
        PC += 1;
        push();
        break;

      case 0xF6:
        PC += 2;
        or_(n8(m_pBus->readBus(PC++)));
        break;

      case 0xF7:
        PC += 1;
        rst(6);
        break;

      case 0xF8:
        PC += 2;
        break;

      case 0xF9:
        PC += 1;
        break;

      case 0xFA:
        PC += 3;
        break;

      case 0xFB:
        PC += 1;
        ei();
        break;

      case 0xFE:
        PC += 2;
        cp(n8(m_pBus->readBus(PC++)));
        break;

      case 0xFF:
        PC += 1;
        rst(7);
        break;
    }
  }
}

void cpu::adc(const r8 r) {
  uint8 c = (F.c() == set) ? 1 : 0;
  (A.loNibble() + r.loNibble() + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);

  A = A + r + c;
  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  (A.value() < r.value() + c) ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::adc(const byte b) {
  uint8 c = F.c() == set ? 1 : 0;
  (A.loNibble() + (b & 0b0000'1111) + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);
  A = A + b + c;

  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  (A.value() < b + c) ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::adc(const n8 n) {
  uint8 c = F.c() == set ? 1 : 0;
  (A.loNibble() + (n.value() & 0b0000'1111) + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);

  A = A + n.value() + c;

  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);

  (A.value() < n.value() + c) ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::add(const r8 r) {
  (A.loNibble() + r.loNibble() + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);

  A = A + r;
  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  A.value() < r.value() ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::add(const byte b) {
  (A.loNibble() + (b & 0b0000'1111) + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);
  A = A + b;

  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  A.value() < b ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::add(const n8 n) {
  (A.loNibble() + (n.value() & 0b0000'1111) + 1 > 0b0000'1111) ? F.h(set) : F.h(reset);

  A = A + n.value();

  A == r8::zero ? F.z(set) : F.z(reset);

  F.n(reset);

  A.value() < n.value() ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::add(const e8 e) {
  SP + e.loNibble() > 0b0000'1111 ? F.h(set) : F.h(reset);
  SP + e.value() > r8::max ? F.c(set) : F.c(reset);
  F.z(reset);
  F.n(reset);

  SP += e.value();
}

void cpu::add(const r16 rr) {
  HL.lo().value() + rr.lo().value() > r8::max ? F.h(set) : F.h(reset);
  HL.value() + rr.value() > r16::max ? F.h(set) : F.h(reset);
  F.n(reset);

  HL = HL + rr;

  m_clock.cycle(2);
}

void cpu::and_(const r8 r) {
  A = A.value() & r.value();

  A == r8::zero ? F.z(set) : F.z(reset);

  F.n(reset);
  F.h(set);
  F.c(reset);

  m_clock.cycle(1);
}

void cpu::and_(const byte b) {
  A = A.value() & b;

  A == r8::zero ? F.z(set) : F.z(reset);

  F.n(reset);
  F.h(set);
  F.c(reset);

  m_clock.cycle(1);
}

void cpu::and_(const n8 n) {
  A = A.value() & n.value();

  A == r8::zero ? F.z(set) : F.z(reset);

  F.n(reset);
  F.h(set);
  F.c(reset);

  m_clock.cycle(1);
}

void cpu::cp(const r8 r) {
  A == r ? F.z(set) : F.z(reset);
  F.n(set);
  r.loNibble() > A.loNibble() ? F.h(set) : F.h(reset);
  A < r ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::cp(const byte b) {
  A == b ? F.z(set) : F.z(reset);
  F.n(set);

  (b & r8::reset_upper) > A.loNibble() ? F.h(set) : F.h(reset);
  A.value() < b ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::cp(const n8 n) {
  A.value() == n.value() ? F.z(set) : F.z(reset);
  F.n(set);
  (n.loNibble() > A.loNibble()) ? F.h(set) : F.h(reset);
  A.value() < n.value() ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::dec(r8 &r) {
  --r;

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(set);
  r == r8::reset_upper ? F.h(set) : F.h(reset);

  m_clock.cycle(1);
}

void cpu::dec(const uint16 uu) {
  byte val = m_pBus->readBus(uu);
  --val;
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(set);
  val == 0b0000'1111 ? F.h(set) : F.h(reset);

  m_clock.cycle(3);
}

void cpu::dec(r16 &rr) {
  rr.lo()--;

  m_clock.cycle(2);
}

void cpu::inc(r8 &r) {
  r.loNibble() == r8::reset_upper ? F.h(set) : F.h(reset);

  ++r;

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);

  m_clock.cycle(1);
}

void cpu::inc(const uint16 uu) {

  byte val = m_pBus->readBus(uu);
  val == 0b0000'1111 ? F.h(set) : F.h(reset);
  ++val;
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(set);

  m_clock.cycle(3);
}

void cpu::inc(r16 &rr) {
  rr.hi()++;

  m_clock.cycle(2);
}

void cpu::or_(const r8 r) {
  A = A.value() | r.value();
  A == r8::zero ? F.z(set) : F.z(reset);

  m_clock.cycle(2);
}

void cpu::or_(const byte b) {
  A = A.value() | b;
  A == r8::zero ? F.z(set) : F.z(reset);

  m_clock.cycle(2);
}

void cpu::or_(const n8 n) {
  A = A.value() | n.value();
  A == r8::zero ? F.z(set) : F.z(reset);

  m_clock.cycle(2);
}

void cpu::sbc(const r8 r) {
  uint8 carry = F.c() == set ? 1 : 0;

  (r + carry) > A.value() ? F.c(set) : F.c(reset);
  A.loNibble() < ((r + carry) & r8::reset_upper) ? F.h(set) : F.h(reset);
  F.n(set);

  A = A - (r + carry);

  A == r8::zero ? F.z(set) : F.z(reset);

  m_clock.cycle(1);
}

void cpu::sbc(const byte b) {
  uint8 carry = F.c() == set ? 1 : 0;

  (b + carry) > A.value() ? F.c(set) : F.c(reset);
  A.loNibble() < ((b + carry) & 0b0000'1111) ? F.h(set) : F.h(reset);
  F.n(set);

  A = A - (b + carry);

  A == r8::zero ? F.z(set) : F.z(reset);

  m_clock.cycle(2);
}

void cpu::sbc(const n8 n) {
  uint8 carry = F.c() == set ? 1 : 0;
  n.value() + carry > A.value() ? F.z(set) : F.z(reset);

  // can i make make this more expressive?
  ((n.value() + carry) & 0b0000'1111) > A.loNibble() ? F.h(set) : F.h(reset);
  F.n(set);

  A = A - (n.value() + 1);

  A == r8::zero ? F.z(set) : F.z(reset);
  m_clock.cycle(2);
}

void cpu::sub(const r8 r) {

  r > A ? F.c(set) : F.c(reset);

  A.loNibble() < r.loNibble() ? F.h(set) : F.h(reset);
  F.n(set);

  A = A - r;

  A == r8::zero ? F.z(set) : F.z(reset);

  m_clock.cycle(1);
}

void cpu::sub(const byte b) {

  b > A.value() ? F.c(set) : F.c(reset);
  A.loNibble() < (b & 0b0000'1111) ? F.h(set) : F.h(reset);
  F.n(set);

  A = A - b;

  A == r8::zero ? F.z(set) : F.z(reset);

  m_clock.cycle(2);
}

void cpu::sub(const n8 n) {
  n.value() > A.value() ? F.z(set) : F.z(reset);

  n.loNibble() > A.loNibble() ? F.h(set) : F.h(reset);
  F.n(set);

  A = A - n.value();

  A == r8::zero ? F.z(set) : F.z(reset);
  m_clock.cycle(2);
}

void cpu::xor_(const r8 r) {
  A = A.value() ^ r.value();

  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  F.c(reset);

  m_clock.cycle(1);
}

void cpu::xor_(const byte b) {
  A = A.value() ^ b;

  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  F.c(reset);

  m_clock.cycle(2);
}

void cpu::xor_(const n8 n) {
  A = A.value() ^ n.value();

  A == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  F.c(reset);

  m_clock.cycle(2);
}

void cpu::bit(const uint8 pos, const r8 r) {
  uint8 test_bit_mask = 0b1 << pos;

  test_bit_mask &r.value() ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(set);

  m_clock.cycle(2);
}

void cpu::bit(const uint8 pos, const byte b) {
  uint8 test_bit_mask = 0b1 << pos;

  test_bit_mask &b ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(set);

  m_clock.cycle(3);
}

void cpu::res(const uint8 pos, r8 &r) {
  uint8 reset_bit_mask = ~(0b1 << pos);
  r = r.value() & reset_bit_mask;

  m_clock.cycle(2);
}

void cpu::res(const uint8 pos, const uint16 uu) {
  uint8 reset_bit_mask = ~(0b1 << pos);

  byte val = m_pBus->readBus(uu);
  val = val & reset_bit_mask;

  m_pBus->writeBus(uu, val);

  m_clock.cycle(4);
}

void cpu::set_(const uint8 pos, r8 &r) {
  uint8 set_bit_mask = (0b1 << pos);
  r = r.value() & set_bit_mask;

  m_clock.cycle(2);
}

void cpu::set_(const uint8 pos, const uint16 uu) {
  uint8 set_bit_mask = (0b1 << pos);

  byte val = m_pBus->readBus(uu);
  val = val & set_bit_mask;

  m_pBus->writeBus(uu, val);

  m_clock.cycle(4);
}

void cpu::swap(r8 &r) {
  // swap lower and upper nibble

  r = (r.loNibble() << 4) | r.hiNibble() >> 4;

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  F.c(reset);

  m_clock.cycle(2);
}

void cpu::swap(const uint16 uu) {
  byte val = m_pBus->readBus(uu);
  val = (val & 0b0000'1111) << 4 | (val & 0b1111'0000) >> 4;
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  F.c(reset);

  m_clock.cycle(4);
}

void cpu::rl(r8 &r) {
  uint8 old_carry = F.c() == set ? 1 : 0;

  r = (r.value() << 1) | old_carry;

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  r.value() & 0b1000'0000 ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}
void cpu::rl(const uint16 uu) {
  uint8 old_carry = F.c() == set ? 1 : 0;

  byte val = m_pBus->readBus(uu);
  val = (val << 1) | old_carry;
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  val & 0b1000'0000 ? F.c(set) : F.c(reset);

  m_clock.cycle(4);
}

void cpu::rla() {
  uint8 old_carry = F.c() == set ? 1 : 0;

  A = (A.value() << 1) | old_carry;

  F.z(reset);
  F.n(reset);
  F.h(reset);
  A.value() & 0b1000'0000 ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::rlc(r8 &r) {
  uint8 old_seventh_bit = r.value() >> 7;
  r = (r.value() << 1) | old_seventh_bit;

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_seventh_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::rlc(const uint16 uu) {
  byte val = m_pBus->readBus(uu);

  uint8 old_seventh_bit = val >> 7;
  val = (val << 1) | old_seventh_bit;
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_seventh_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(4);
}

void cpu::rlca() {

  uint8 old_seventh_bit = A.value() >> 7;
  A = (A.value() << 1) | old_seventh_bit;

  F.z(reset);
  F.n(reset);
  F.h(reset);
  old_seventh_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::rr(r8 &r) {
  uint8 carry = F.c() == set ? 1 : 0;
  uint8 old_first = r.value() & 0b0000'0001;
  r = (r.value() >> 1) | (carry << 7);

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}
void cpu::rr(const uint16 uu) {
  byte val = m_pBus->readBus(uu);

  uint8 old_first = val & 0b0000'0001;
  uint8 carry = F.c() == set ? 1 : 0;

  val = (val >> 1) | (carry << 7);

  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first ? F.c(set) : F.c(reset);

  m_clock.cycle(4);
}

void cpu::rra() {
  uint8 carry = F.c() == set ? 1 : 0;
  uint8 old_first = A.value() & 0b0000'0001;
  A = (A.value() >> 1) | (carry << 7);

  F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::rrc(r8 &r) {
  uint8 old_first = r.value() & 0b0000'0001;

  r = (r.value() >> 1) | (old_first << 7);

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::rrc(const uint16 uu) {
  byte val = m_pBus->readBus(uu);
  uint8 old_first = val & 0b0000'0001;

  val = (val >> 1) | (old_first << 7);
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first ? F.c(set) : F.c(reset);

  m_clock.cycle(4);
}

void cpu::rrca() {
  uint8 old_first = A.value() & 0b0000'0001;

  A = (A.value() >> 1) | (old_first << 7);

  F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first ? F.c(set) : F.c(reset);

  m_clock.cycle(1);
}

void cpu::sla(r8 &r) {
  uint8 old_seventh_bit = r.value() >> 7;

  r = r.value() << 1;

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_seventh_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::sla(const uint16 uu) {
  byte val = m_pBus->readBus(uu);
  uint8 old_seventh_bit = val >> 7;

  val = val << 1;
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_seventh_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(4);
}

void cpu::sra(r8 &r) {
  uint8 old_first_bit = A.value() & 0b0000'0001;
  uint8 old_seventh_bit = r.value() >> 7;

  r = (r.value() >> 1) | (old_seventh_bit << 7);

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::sra(const uint16 uu) {
  byte val = m_pBus->readBus(uu);

  uint8 old_first_bit = val & 0b0000'0001;
  uint8 old_seventh_bit = val >> 7;

  val = (val >> 1) | (old_seventh_bit << 7);
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(4);
}

void cpu::srl(r8 &r) {
  uint8 old_first_bit = A.value() & 0b0000'0001;

  r = r.value() >> 1;

  r == r8::zero ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(2);
}

void cpu::srl(const uint16 uu) {
  byte val = m_pBus->readBus(uu);

  uint8 old_first_bit = val & 0b0000'0001;

  val = val >> 1;
  m_pBus->writeBus(uu, val);

  val == 0 ? F.z(set) : F.z(reset);
  F.n(reset);
  F.h(reset);
  old_first_bit ? F.c(set) : F.c(reset);

  m_clock.cycle(4);
}

void cpu::ld(r8 &l, const r8 &r) {
  l = r;
  m_clock.cycle(1);
}

void cpu::ld(r8 &r, const n8 &n) {
  r = n;

  m_clock.cycle(2);
}
void cpu::ld(const uint16 uu, const r8 &r) {
  m_pBus->writeBus(uu, r.value());

  m_clock.cycle(2);
}

void cpu::ld(r8 &r, const uint16 uu) {
  r = m_pBus->readBus(uu);

  m_clock.cycle(2);
}

void cpu::ldi(r16 &rr, const r8 r) {
  m_pBus->writeBus(rr.value(), r.value());
  rr++;

  m_clock.cycle(2);
}

void cpu::ldi(r8 &r, r16 &rr) {
  r = m_pBus->readBus(rr.value());
  rr++;

  m_clock.cycle(2);
}

void cpu::ldd(r16 &rr, const r8 r) {
  m_pBus->writeBus(rr.value(), r.value());
  rr--;

  m_clock.cycle(2);
}

void cpu::ldd(r8 &r, r16 &rr) {
  r = m_pBus->readBus(rr.value());
  rr--;

  m_clock.cycle(2);
}

void cpu::ld(r16 &rr, const n16 nn) {
  rr = nn;

  m_clock.cycle(3);
}

void cpu::ldio(const uint16 nn, const r8 r) {
  m_pBus->writeBus(nn, r.value());

  // Fix: if nn is 0xFF00 + C, then cycle 2 times
  m_clock.cycle(3);
}

void cpu::ldio(r8 &r, const uint16 nn) {
  r = m_pBus->readBus(nn);

  // Fix: if nn is 0xFF00 + C, then cycle 2 times
  m_clock.cycle(3);
}

void cpu::call(n16 n) {
  // (SP - 1) <- PCH
  // (SP - 2) <- PCL
  // PC <- nn
  // SP <- SP-2

  m_pBus->writeBus(SP - 1, PC & r16::reset_lower >> 8);
  m_pBus->writeBus(SP - 2, PC & r16::reset_upper);

  byte lo = m_pBus->readBus(PC++);
  byte hi = m_pBus->readBus(PC++);
  uint16 nn = hi << 8 | lo;

  PC = nn;
  SP = SP - 2;

  m_clock.cycle(6);
}
// clang-format off
void cpu::call(cc c, n16 n) {
  if (c == cc::Z && F.z() == set    || 
      c == cc::NZ && F.z() == reset || 
      c == cc::C && F.c() == set    ||
      c == cc::NC && F.c() == reset) {
    // clang-format on

    call(n);

    m_clock.cycle(6);
  } else {
    m_clock.cycle(3);
  }
}

void cpu::jp() {
  PC = HL.value();

  m_clock.cycle(1);
}

void cpu::jp(const n16 nn) {
  PC = nn.value();

  m_clock.cycle(4);
}
// clang-format off
void cpu::jp(const cc c, const n16 nn) {
  if (c == cc::Z && F.z() == set    || 
      c == cc::NZ && F.z() == reset || 
      c == cc::C && F.c() == set    ||
      c == cc::NC && F.c() == reset) {
    // clang-format on
    jp(nn);

    m_clock.cycle(4);
  } else {
    m_clock.cycle(3);
  }
}
void cpu::jr(const e8 e) {
  PC = e.value();

  m_clock.cycle(3);
}

void cpu::jr(const cc c, const e8 e) {
  // clang-format off
  if (c == cc::Z && F.z() == set    || 
      c == cc::NZ && F.z() == reset || 
      c == cc::C && F.c() == set    ||
      c == cc::NC && F.c() == reset) {
    // clang-format on]
   jr(e);

    m_clock.cycle(3);
  } else {
    m_clock.cycle(2);
  }
}

void cpu::ret() {
    byte lo = m_pBus->readBus(SP);
    byte hi = m_pBus->readBus(SP+1);
    PC = hi << 8 | lo;

    SP += 2;

    m_clock.cycle(4);
  }

void cpu::ret(cc c) {
      // clang-format off
  if (c == cc::Z && F.z() == set    || 
      c == cc::NZ && F.z() == reset || 
      c == cc::C && F.c() == set    ||
      c == cc::NC && F.c() == reset) {
    // clang-format on
    ret();
    m_clock.cycle(5);
  } else {
    m_clock.cycle(2);
  }
}

void cpu::reti() {
  IME = set;
  ret();

  m_clock.cycle(4);
}

void cpu::rst(const uint8 u) {
  m_pBus->writeBus(SP - 1, PC & r16::reset_lower >> 8);
  m_pBus->writeBus(SP - 2, PC & r16::reset_upper);
  SP -= 2;

  PC = PC & r16::reset_upper;
  PC = vec.at(u);

  m_clock.cycle(4);
}

void cpu::pop() {
  F = m_pBus->readBus(SP);
  A = m_pBus->readBus(SP + 1);
  SP += 2;

  m_clock.cycle(3);
}

void cpu::pop(r16 &rr) {
  rr.lo() = m_pBus->readBus(SP);
  rr.hi() = m_pBus->readBus(SP + 1);
  SP += 2;

  m_clock.cycle(3);
}

void cpu::push() {
  m_pBus->writeBus(SP - 1, A.value());
  m_pBus->writeBus(SP - 2, F.value());
  SP -= 2;

  m_clock.cycle(4);
}

void cpu::push(r16 &rr) {
  m_pBus->writeBus(SP - 1, rr.hi().value());
  m_pBus->writeBus(SP - 2, rr.lo().value());
  SP -= 2;

  m_clock.cycle(4);
}

void cpu::ccf() {
  F.n(reset);
  F.h(reset);
  F.c(!F.c());

  m_clock.cycle(1);
}

void cpu::cpl() {
  A = ~A.value();
  F.n(set);
  F.h(set);

  m_clock.cycle(1);
}
void cpu::di() {
  IME = reset;

  m_clock.cycle(1);
}

void cpu::ei() {
  IME = set;

  m_clock.cycle(1);
}

void cpu::scf() {
  F.n(reset);
  F.h(reset);
  F.c(set);

  m_clock.cycle(1);
}
}
