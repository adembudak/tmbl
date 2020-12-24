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

namespace tmbl {
cpu::cpu(bus &bus_, registers &reg_, interrupts &intr_)
    : m_bus(bus_), m_reg(reg_), m_intr(intr_), KEY1(m_reg.getAt(0xFF4D)) {

  // Put initial values of the registers
  // see: https://gbdev.io/pandocs/#power-up-sequence

  A = 0x01;
  F = 0xB0;

  BC.hi() = 0x00;
  BC.lo() = 0x13;

  DE.hi() = 0x00;
  DE.lo() = 0xD8;

  HL.hi() = 0x01;
  HL.lo() = 0x4D;

  SP = 0xFFFE;
}

// see: double speed mode switch procedure
// https://archive.org/details/GameBoyProgManVer1.1/page/n35/mode/1up

void cpu::enableDoubleSpeedMode() {
  if ((KEY1 & 0b0100'0000) && (KEY1 & 0b0000'0001)) {
    m_clock.enableDoubleSpeedMode(true);
    m_bus.writeBus(0xFF0F /*IF*/, 0);
    m_bus.writeBus(0xFFFF /*IE*/, 0);
    m_reg.getAt(0xFF00 /*P1*/) |= 0b0011'0000;
    stop();
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

  auto fetch_byte = [&] { return m_bus.readBus(PC++); };
  auto fetch_word = [&] { return fetch_byte() | (fetch_byte() << 8); };

  if (IME) {
    if (m_intr.IE() && m_intr.IF() && 0b0001'1111) {
      if (m_intr.VBlank_IRQ && m_intr.VBlank_Enabled) {
        m_intr.VBlank_IRQ = false;
        di();
        call(intr_vec[0]);
      } else if (m_intr.LCDC_Status_IRQ && m_intr.LCDC_Status_Enabled) {
        m_intr.LCDC_Status_IRQ = false;
        di();
        call(intr_vec[1]);
      } else if (m_intr.Timer_Overflow_IRQ && m_intr.Timer_Overflow_Enabled) {
        m_intr.Timer_Overflow_IRQ = false;
        di();
        call(intr_vec[2]);
      } else if (m_intr.Serial_Transfer_Completion_IRQ &&
                 m_intr.Serial_Transfer_Completion_Enabled) {
        m_intr.Serial_Transfer_Completion_IRQ = false;
        di();
        call(intr_vec[3]);
      } else if (m_intr.Button_Pressed_IRQ && m_intr.Button_Pressed_Enabled) {
        m_intr.Button_Pressed_IRQ = false;
        di();
        call(intr_vec[4]);
      }
    }
  }

  switch (fetch_byte()) {
    case 0x00:
      nop();
      break;

    case 0x01:
      ld(BC, n16(fetch_word()));
      break;

    case 0x02:
      ld(BC.value(), A);
      break;

    case 0x03:
      inc(BC);
      break;

    case 0x04:
      inc(BC.hi());
      break;

    case 0x05:
      dec(BC.hi());
      break;

    case 0x06:
      ld(BC.hi(), n8(fetch_byte()));
      break;

    case 0x07:
      rlca();
      break;

    case 0x08:
      m_bus.writeBus(n16(fetch_word()).value(), SP);
      m_clock.cycle(4);
      break;

    case 0x09:
      add(BC);
      break;

    case 0x0A:
      ld(A, BC.value());
      break;

    case 0x0B:
      dec(BC);
      break;

    case 0x0C:
      inc(BC.lo());
      break;

    case 0x0D:
      add(BC.lo());
      break;

    case 0x0E:
      ld(BC.lo(), n8(fetch_byte()));
      break;

    case 0x0F:
      rrca();
      break;

    case 0x10:
      stop();
      break;

    case 0x11:
      ld(DE, n16(fetch_word()));
      break;

    case 0x12:
      ld(DE.value(), A);
      break;

    case 0x13:
      inc(DE);
      break;

    case 0x14:
      inc(DE.hi());
      break;

    case 0x15:
      dec(DE.hi());
      break;

    case 0x16:
      ld(DE.hi(), n8(fetch_byte()));
      break;

    case 0x17:
      rla();
      break;

    case 0x18:
      PC += 1;
      jr(e8(m_bus.readBus(PC++)));
      break;

    case 0x19:
      add(DE);
      break;

    case 0x1A:
      ld(A, DE.value());
      break;

    case 0x1B:
      dec(DE);
      break;

    case 0x1C:
      inc(DE.lo());
      break;

    case 0x1D:
      dec(DE.hi());
      break;

    case 0x1E:
      ld(DE.lo(), n8(fetch_byte()));
      break;

    case 0x1F:
      rra();
      break;

    case 0x20:
      PC += 1;
      jr(cc::NZ, e8(m_bus.readBus(PC++)));
      break;

    case 0x21:
      ld(HL, n16(fetch_word()));
      break;

    case 0x22:
      ldi(HL, A);
      break;

    case 0x23:
      inc(HL);
      break;

    case 0x24:
      inc(HL.hi());
      break;

    case 0x25:
      dec(HL.hi());
      break;

    case 0x26:
      ld(HL.hi(), n8(fetch_byte()));
      break;

    case 0x27:
      daa();
      break;

    case 0x28:
      PC += 1;
      jr(cc::Z, e8(m_bus.readBus(PC++)));
      break;

    case 0x29:
      add(HL);
      break;

    case 0x2A:
      ldi(A, HL);
      break;

    case 0x2B:
      dec(HL);
      break;

    case 0x2C:
      inc(HL.lo());
      break;

    case 0x2D:
      dec(HL.lo());
      break;

    case 0x2E:
      ld(HL.lo(), n8(fetch_byte()));
      break;

    case 0x2F:
      cpl();
      break;

    case 0x30:
      PC += 1;
      jr(cc::NC, e8(m_bus.readBus(PC++)));
      break;

    case 0x31:
      SP = n16(fetch_word()).value();
      m_clock.cycle(3);
      break;

    case 0x32:
      ldd(HL, A);
      break;

    case 0x33:

      ++SP;
      m_clock.cycle(2);
      break;

    case 0x34:
      inc(HL.value());
      break;

    case 0x35:
      dec(HL.value());
      break;

    case 0x36:
      m_bus.writeBus(HL.value(), n8(fetch_byte()).value());
      m_clock.cycle(3);
      break;

    case 0x37:
      scf();
      break;

    case 0x38:
      PC += 1;
      jr(cc::C, e8(m_bus.readBus(PC++)));
      break;

    case 0x39: {

      uint8 t_old_lo_reg_val = HL.lo().value();
      uint16 t_old_reg_val = HL.value();

      HL.lo() = SP & 0x00FF;
      HL.hi() = (SP & 0xFF00) >> 8;

      F.n(reset);
      t_old_reg_val > HL.value() ? F.c(set) : F.c(reset);
      t_old_lo_reg_val > HL.lo().value() ? F.h(set) : F.h(reset);

      m_clock.cycle(2);
    } break;

    case 0x3A:
      ldd(A, HL);
      break;

    case 0x3B:
      SP--;
      break;

    case 0x3C:
      inc(A);
      break;

    case 0x3D:
      dec(A);
      break;

    case 0x3E:
      ld(A, n8(fetch_byte()));
      break;

    case 0x3F:
      ccf();
      break;

    case 0x40:
      ld(BC.hi(), BC.hi());
      break;

    case 0x41:
      ld(BC.hi(), BC.lo());
      break;

    case 0x42:
      ld(BC.hi(), DE.hi());
      break;

    case 0x43:
      ld(BC.hi(), DE.lo());
      break;

    case 0x44:
      ld(BC.hi(), HL.hi());
      break;

    case 0x45:
      ld(BC.hi(), HL.lo());
      break;

    case 0x46:
      ld(BC.hi(), HL.value());
      break;

    case 0x47:
      ld(BC.hi(), A);
      break;

    case 0x48:
      ld(BC.lo(), BC.hi());
      break;

    case 0x49:
      ld(BC.lo(), BC.lo());
      break;

    case 0x4A:
      ld(BC.lo(), DE.hi());
      break;

    case 0x4B:
      ld(BC.lo(), DE.lo());
      break;

    case 0x4C:
      ld(BC.lo(), HL.hi());
      break;

    case 0x4D:
      ld(BC.lo(), HL.lo());
      break;

    case 0x4E:
      ld(BC.lo(), HL.value());
      break;

    case 0x4F:
      ld(BC.lo(), A);
      break;

    case 0x50:
      ld(DE.hi(), BC.hi());
      break;

    case 0x51:
      ld(DE.hi(), BC.lo());
      break;

    case 0x52:
      ld(DE.hi(), DE.hi());
      break;

    case 0x53:
      ld(DE.hi(), DE.lo());
      break;

    case 0x54:
      ld(DE.hi(), HL.hi());
      break;

    case 0x55:
      ld(DE.hi(), HL.lo());
      break;

    case 0x56:
      ld(DE.hi(), HL.value());
      break;

    case 0x57:
      ld(DE.hi(), A);
      break;

    case 0x58:
      ld(DE.lo(), BC.hi());
      break;

    case 0x59:
      ld(DE.lo(), BC.lo());
      break;

    case 0x5A:
      ld(DE.lo(), DE.hi());
      break;

    case 0x5B:
      ld(DE.lo(), DE.lo());
      break;

    case 0x5C:
      ld(DE.lo(), HL.hi());
      break;

    case 0x5D:
      ld(DE.lo(), HL.lo());
      break;

    case 0x5E:
      ld(DE.lo(), HL.value());
      break;

    case 0x5F:
      ld(DE.lo(), A);
      break;

    case 0x60:
      ld(HL.hi(), BC.hi());
      break;

    case 0x61:
      ld(HL.hi(), BC.lo());
      break;

    case 0x62:
      ld(HL.hi(), DE.hi());
      break;

    case 0x63:
      ld(HL.hi(), DE.lo());
      break;

    case 0x64:
      ld(HL.hi(), HL.hi());
      break;

    case 0x65:
      ld(HL.hi(), HL.lo());
      break;

    case 0x66:
      ld(HL.hi(), HL.value());
      break;

    case 0x67:
      ld(HL.hi(), A);
      break;

    case 0x68:
      ld(HL.lo(), BC.hi());
      break;

    case 0x69:
      ld(HL.lo(), BC.lo());
      break;

    case 0x6A:
      ld(HL.lo(), DE.hi());
      break;

    case 0x6B:
      ld(HL.lo(), DE.lo());
      break;

    case 0x6C:
      ld(HL.lo(), HL.hi());
      break;

    case 0x6D:
      ld(HL.lo(), HL.lo());
      break;

    case 0x6E:
      ld(HL.lo(), HL.value());
      break;

    case 0x6F:
      ld(HL.lo(), A);
      break;

    case 0x70:
      ld(HL.value(), BC.hi());
      break;

    case 0x71:
      ld(HL.value(), BC.lo());
      break;

    case 0x72:
      ld(HL.value(), DE.hi());
      break;

    case 0x73:
      ld(HL.value(), DE.lo());
      break;

    case 0x74:
      ld(HL.value(), HL.hi());
      break;

    case 0x75:
      ld(HL.value(), HL.lo());
      break;

    case 0x76:
      halt();
      break;

    case 0x77:
      ld(HL.value(), A);
      break;

    case 0x78:
      ld(A, BC.hi());
      break;

    case 0x79:
      ld(A, BC.lo());
      break;

    case 0x7A:
      ld(A, DE.hi());
      break;

    case 0x7B:
      ld(A, DE.lo());
      break;

    case 0x7C:
      ld(A, HL.hi());
      break;

    case 0x7D:
      ld(A, HL.lo());
      break;

    case 0x7E:
      ld(A, HL.value());
      break;

    case 0x7F:
      ld(A, A);
      break;

    case 0x80:
      add(BC.hi());
      break;

    case 0x81:
      add(BC.lo());
      break;

    case 0x82:
      add(DE.hi());
      break;

    case 0x83:
      add(DE.lo());
      break;

    case 0x84:
      add(HL.hi());
      break;

    case 0x85:
      add(HL.lo());
      break;

    case 0x86:
      add(m_bus.readBus(HL.value()));
      break;

    case 0x87:
      add(A);
      break;

    case 0x88:
      adc(BC.hi());
      break;

    case 0x89:
      adc(BC.lo());
      break;

    case 0x8A:
      adc(DE.hi());
      break;

    case 0x8B:
      adc(DE.lo());
      break;

    case 0x8C:
      adc(HL.hi());
      break;

    case 0x8D:
      adc(HL.lo());
      break;

    case 0x8E:
      adc(m_bus.readBus(HL.value()));
      break;

    case 0x8F:
      adc(A);
      break;

    case 0x90:
      sub(BC.hi());
      break;

    case 0x91:
      sub(BC.lo());
      break;

    case 0x92:
      sub(DE.hi());
      break;

    case 0x93:
      sub(DE.lo());
      break;

    case 0x94:
      sub(HL.hi());
      break;

    case 0x95:
      sub(HL.lo());
      break;

    case 0x96:
      sub(m_bus.readBus(HL.value()));
      break;

    case 0x97:
      sub(A);
      break;

    case 0x98:
      sbc(BC.hi());
      break;

    case 0x99:
      sbc(BC.lo());
      break;

    case 0x9A:
      sbc(DE.hi());
      break;

    case 0x9B:
      sbc(DE.lo());
      break;

    case 0x9C:
      sbc(HL.hi());
      break;

    case 0x9D:
      sbc(HL.lo());
      break;

    case 0x9E:
      sbc(m_bus.readBus(HL.value()));
      break;

    case 0x9F:
      sbc(A);
      break;

    case 0xA0:
      and_(BC.hi());
      break;

    case 0xA1:
      and_(BC.lo());
      break;

    case 0xA2:
      and_(DE.hi());
      break;

    case 0xA3:
      and_(DE.hi());
      break;

    case 0xA4:
      and_(HL.hi());
      break;

    case 0xA5:
      and_(HL.lo());
      break;

    case 0xA6:
      and_(m_bus.readBus(HL.value()));
      break;

    case 0xA7:
      and_(A);
      break;

    case 0xA8:
      xor_(BC.hi());
      break;

    case 0xA9:
      xor_(BC.lo());
      break;

    case 0xAA:
      xor_(DE.hi());
      break;

    case 0xAB:
      xor_(DE.lo());
      break;

    case 0xAC:
      xor_(HL.hi());
      break;

    case 0xAD:
      xor_(HL.lo());
      break;

    case 0xAE:
      xor_(m_bus.readBus(HL.value()));
      break;

    case 0xAF:
      xor_(A);
      break;

    case 0xB0:
      or_(BC.hi());
      break;

    case 0xB1:
      or_(BC.lo());
      break;

    case 0xB2:
      or_(DE.hi());
      break;

    case 0xB3:
      or_(DE.lo());
      break;

    case 0xB4:
      or_(HL.hi());
      break;

    case 0xB5:
      or_(HL.lo());
      break;

    case 0xB6:
      or_(m_bus.readBus(HL.value()));
      break;

    case 0xB7:
      or_(A);
      break;

    case 0xB8:
      cp(BC.hi());
      break;

    case 0xB9:
      cp(BC.lo());
      break;

    case 0xBA:
      cp(DE.hi());
      break;

    case 0xBB:
      cp(DE.lo());
      break;

    case 0xBC:
      cp(HL.hi());
      break;

    case 0xBD:
      cp(HL.lo());
      break;

    case 0xBE:
      cp(m_bus.readBus(HL.value()));
      break;

    case 0xBF:
      cp(A);
      break;

    case 0xC0:
      ret(cc::NZ);
      break;

    case 0xC1:
      pop(BC);
      break;

    case 0xC2:
      jp(cc::NZ, n16(fetch_word()));
      break;

    case 0xC3:
      jp(n16(fetch_word()));
      break;

    case 0xC4:
      call(cc::NZ, n16(fetch_word()));
      break;

    case 0xC5:
      push(BC);
      break;

    case 0xC6:
      PC += 1;
      add(m_bus.readBus(PC++));
      break;

    case 0xC7:
      rst(0);
      break;

    case 0xC8:
      ret(cc::Z);
      break;

    case 0xC9:
      ret();
      break;

    case 0xCA:
      jp(cc::Z, n16(fetch_word()));
      break;

    case 0xCB: // 0xCB Prefixed:

      switch (fetch_byte()) {
        case 0x00:
          rlc(BC.hi());
          break;

        case 0x01:
          rlc(BC.lo());
          break;

        case 0x02:
          rlc(DE.hi());
          break;

        case 0x03:
          rlc(DE.lo());
          break;

        case 0x04:
          rlc(HL.hi());
          break;

        case 0x05:
          rlc(HL.lo());
          break;

        case 0x06:
          rlc(HL.value());
          break;

        case 0x07:
          rlc(A);
          break;

        case 0x08:
          rrc(BC.hi());
          break;

        case 0x09:
          rrc(BC.lo());
          break;

        case 0x0A:
          rrc(DE.hi());
          break;

        case 0x0B:
          rrc(DE.lo());
          break;

        case 0x0C:
          rrc(HL.hi());
          break;

        case 0x0D:
          rrc(HL.lo());
          break;

        case 0x0E:
          rrc(HL.value());
          break;

        case 0x0F:
          rrc(A);
          break;

        case 0x10:
          rl(BC.hi());
          break;

        case 0x11:
          rl(BC.lo());
          break;

        case 0x12:
          rl(DE.hi());
          break;

        case 0x13:
          rl(DE.lo());
          break;

        case 0x14:
          rl(HL.hi());
          break;

        case 0x15:
          rl(HL.lo());
          break;

        case 0x16:
          rl(HL.value());
          break;

        case 0x17:
          rl(A);
          break;

        case 0x18:
          rr(BC.hi());
          break;

        case 0x19:
          rr(BC.lo());
          break;

        case 0x1A:
          rr(DE.hi());
          break;

        case 0x1B:
          rr(DE.lo());
          break;

        case 0x1C:
          rr(HL.hi());
          break;

        case 0x1D:
          rr(HL.lo());
          break;

        case 0x1E:
          rr(HL.value());
          break;

        case 0x1F:
          rr(A);
          break;

        case 0x20:
          sla(BC.hi());
          break;

        case 0x21:
          sla(BC.lo());
          break;

        case 0x22:
          sla(DE.hi());
          break;

        case 0x23:
          sla(DE.lo());
          break;

        case 0x24:
          sla(HL.hi());
          break;

        case 0x25:
          sla(HL.lo());
          break;

        case 0x26:
          sla(HL.value());
          break;

        case 0x27:
          sla(A);
          break;

        case 0x28:
          sra(BC.hi());
          break;

        case 0x29:
          sra(BC.lo());
          break;

        case 0x2A:
          sra(DE.hi());
          break;

        case 0x2B:
          sra(DE.lo());
          break;

        case 0x2C:
          sra(HL.hi());
          break;

        case 0x2D:
          sra(HL.lo());
          break;

        case 0x2E:
          sra(HL.value());
          break;

        case 0x2F:
          sra(A);
          break;

        case 0x30:
          swap(BC.hi());
          break;

        case 0x31:
          swap(BC.lo());
          break;

        case 0x32:
          swap(DE.hi());
          break;

        case 0x33:
          swap(DE.lo());
          break;

        case 0x34:
          swap(HL.hi());
          break;

        case 0x35:
          swap(HL.lo());
          break;

        case 0x36:
          swap(HL.value());
          break;

        case 0x37:
          swap(A);
          break;

        case 0x38:
          srl(BC.hi());
          break;

        case 0x39:
          srl(BC.lo());
          break;

        case 0x3A:
          srl(DE.hi());
          break;

        case 0x3B:
          srl(DE.lo());
          break;

        case 0x3C:
          srl(HL.hi());
          break;

        case 0x3D:
          srl(HL.lo());
          break;

        case 0x3E:
          srl(HL.value());
          break;

        case 0x3F:
          srl(A);
          break;

        case 0x40:
          bit(0, BC.hi());
          break;

        case 0x41:
          bit(0, BC.lo());
          break;

        case 0x42:
          bit(0, DE.hi());
          break;

        case 0x43:
          bit(0, DE.lo());
          break;

        case 0x44:
          bit(0, HL.hi());
          break;

        case 0x45:
          bit(0, HL.lo());
          break;

        case 0x46:
          bit(0, m_bus.readBus(HL.value()));
          break;

        case 0x47:
          bit(0, A);
          break;

        case 0x48:
          bit(1, BC.hi());
          break;

        case 0x49:
          bit(1, BC.lo());
          break;

        case 0x4A:
          bit(1, DE.hi());
          break;

        case 0x4B:
          bit(1, DE.lo());
          break;

        case 0x4C:
          bit(1, HL.hi());
          break;

        case 0x4D:
          bit(1, HL.lo());
          break;

        case 0x4E:
          bit(1, m_bus.readBus(HL.value()));
          break;

        case 0x4F:
          bit(1, A);
          break;

        case 0x50:
          bit(2, BC.hi());
          break;

        case 0x51:
          bit(2, BC.lo());
          break;

        case 0x52:
          bit(2, DE.hi());
          break;

        case 0x53:
          bit(2, DE.lo());
          break;

        case 0x54:
          bit(2, HL.hi());
          break;

        case 0x55:
          bit(2, HL.lo());
          break;

        case 0x56:
          bit(2, m_bus.readBus(HL.value()));
          break;

        case 0x57:
          bit(2, A);
          break;

        case 0x58:
          bit(3, BC.hi());
          break;

        case 0x59:
          bit(3, BC.lo());
          break;

        case 0x5A:
          bit(3, DE.hi());
          break;

        case 0x5B:
          bit(3, DE.lo());
          break;

        case 0x5C:
          bit(3, HL.hi());
          break;

        case 0x5D:
          bit(3, HL.lo());
          break;

        case 0x5E:
          bit(3, m_bus.readBus(HL.value()));
          break;

        case 0x5F:
          bit(3, A);
          break;

        case 0x60:
          bit(4, BC.hi());
          break;

        case 0x61:
          bit(4, BC.lo());
          break;

        case 0x62:
          bit(4, DE.hi());
          break;

        case 0x63:
          bit(4, DE.lo());
          break;

        case 0x64:
          bit(4, HL.hi());
          break;

        case 0x65:
          bit(4, HL.lo());
          break;

        case 0x66:
          bit(4, m_bus.readBus(HL.value()));
          break;

        case 0x67:
          bit(4, A);
          break;

        case 0x68:
          bit(5, BC.hi());
          break;

        case 0x69:
          bit(5, BC.lo());
          break;

        case 0x6A:
          bit(5, DE.hi());
          break;

        case 0x6B:
          bit(5, DE.lo());
          break;

        case 0x6C:
          bit(5, HL.hi());
          break;

        case 0x6D:
          bit(5, HL.lo());
          break;

        case 0x6E:
          bit(5, m_bus.readBus(HL.value()));
          break;

        case 0x6F:
          bit(5, A);
          break;

        case 0x70:
          bit(6, BC.hi());
          break;

        case 0x71:
          bit(6, BC.lo());
          break;

        case 0x72:
          bit(6, DE.hi());
          break;

        case 0x73:
          bit(6, DE.lo());
          break;

        case 0x74:
          bit(6, HL.hi());
          break;

        case 0x75:
          bit(6, HL.lo());
          break;

        case 0x76:
          bit(6, m_bus.readBus(HL.value()));
          break;

        case 0x77:
          bit(6, A);
          break;

        case 0x78:
          bit(7, BC.hi());
          break;

        case 0x79:
          bit(7, BC.lo());
          break;

        case 0x7A:
          bit(7, DE.hi());
          break;

        case 0x7B:
          bit(7, DE.lo());
          break;

        case 0x7C:
          bit(7, HL.hi());
          break;

        case 0x7D:
          bit(7, HL.lo());
          break;

        case 0x7E:
          bit(7, m_bus.readBus(HL.value()));
          break;

        case 0x7F:
          bit(7, A);
          break;

        case 0x80:
          res(0, BC.hi());
          break;

        case 0x81:
          res(0, BC.lo());
          break;

        case 0x82:
          res(0, DE.hi());
          break;

        case 0x83:
          res(0, DE.lo());
          break;

        case 0x84:
          res(0, HL.hi());
          break;

        case 0x85:
          res(0, HL.lo());
          break;

        case 0x86:
          res(0, HL.value());
          break;

        case 0x87:
          res(0, A);
          break;

        case 0x88:
          res(1, BC.hi());
          break;

        case 0x89:
          res(1, BC.lo());
          break;

        case 0x8A:
          res(1, DE.hi());
          break;

        case 0x8B:
          res(1, DE.lo());
          break;

        case 0x8C:
          res(1, HL.hi());
          break;

        case 0x8D:
          res(1, HL.lo());
          break;

        case 0x8E:
          res(1, HL.value());
          break;

        case 0x8F:
          res(1, A);
          break;

        case 0x90:
          res(2, BC.hi());
          break;

        case 0x91:
          res(2, BC.lo());
          break;

        case 0x92:
          res(2, DE.hi());
          break;

        case 0x93:
          res(2, DE.lo());
          break;

        case 0x94:
          res(2, HL.hi());
          break;

        case 0x95:
          res(2, HL.lo());
          break;

        case 0x96:
          res(2, HL.value());
          break;

        case 0x97:
          res(2, A);
          break;

        case 0x98:
          res(3, BC.hi());
          break;

        case 0x99:
          res(3, BC.lo());
          break;

        case 0x9A:
          res(3, DE.hi());
          break;

        case 0x9B:
          res(3, DE.lo());
          break;

        case 0x9C:
          res(3, HL.hi());
          break;

        case 0x9D:
          res(3, HL.lo());
          break;

        case 0x9E:
          res(3, HL.value());
          break;

        case 0x9F:
          res(3, A);
          break;

        case 0xA0:
          res(4, BC.hi());
          break;

        case 0xA1:
          res(4, BC.lo());
          break;

        case 0xA2:
          res(4, DE.hi());
          break;

        case 0xA3:
          res(4, DE.lo());
          break;

        case 0xA4:
          res(4, HL.hi());
          break;

        case 0xA5:
          res(4, HL.lo());
          break;

        case 0xA6:
          res(4, HL.value());
          break;

        case 0xA7:
          res(4, A);
          break;

        case 0xA8:
          res(5, BC.hi());
          break;

        case 0xA9:
          res(5, BC.lo());
          break;

        case 0xAA:
          res(5, DE.hi());
          break;

        case 0xAB:
          res(5, DE.lo());
          break;

        case 0xAC:
          res(5, HL.hi());
          break;

        case 0xAD:
          res(5, HL.lo());
          break;

        case 0xAE:
          res(5, HL.value());
          break;

        case 0xAF:
          res(5, A);
          break;

        case 0xB0:
          res(6, BC.hi());
          break;

        case 0xB1:
          res(6, BC.lo());
          break;

        case 0xB2:
          res(6, DE.hi());
          break;

        case 0xB3:
          res(6, DE.lo());
          break;

        case 0xB4:
          res(6, HL.hi());
          break;

        case 0xB5:
          res(6, HL.lo());
          break;

        case 0xB6:
          res(6, HL.value());
          break;

        case 0xB7:
          res(6, A);
          break;

        case 0xB8:
          res(7, BC.hi());
          break;

        case 0xB9:
          res(7, BC.lo());
          break;

        case 0xBA:
          res(7, DE.hi());
          break;

        case 0xBB:
          res(7, DE.lo());
          break;

        case 0xBC:
          res(7, HL.hi());
          break;

        case 0xBD:
          res(7, HL.lo());
          break;

        case 0xBE:
          res(7, HL.value());
          break;

        case 0xBF:
          res(7, A);
          break;

        case 0xC0:
          set_(0, BC.hi());
          break;

        case 0xC1:
          set_(0, BC.lo());
          break;

        case 0xC2:
          set_(0, DE.hi());
          break;

        case 0xC3:
          set_(0, DE.lo());
          break;

        case 0xC4:
          set_(0, HL.hi());
          break;

        case 0xC5:
          set_(0, HL.lo());
          break;

        case 0xC6:
          set_(0, HL.value());
          break;

        case 0xC7:
          set_(0, A);
          break;

        case 0xC8:
          set_(1, BC.hi());
          break;

        case 0xC9:
          set_(1, BC.lo());
          break;

        case 0xCA:
          set_(1, DE.hi());
          break;

        case 0xCB:
          set_(1, DE.lo());
          break;

        case 0xCC:
          set_(1, HL.hi());
          break;

        case 0xCD:
          set_(1, HL.lo());
          break;

        case 0xCE:
          set_(1, HL.value());
          break;

        case 0xCF:
          set_(1, A);
          break;

        case 0xD0:
          set_(2, BC.hi());
          break;

        case 0xD1:
          set_(2, BC.lo());
          break;

        case 0xD2:
          set_(2, DE.hi());
          break;

        case 0xD3:
          set_(2, DE.lo());
          break;

        case 0xD4:
          set_(2, HL.hi());
          break;

        case 0xD5:
          set_(2, HL.lo());
          break;

        case 0xD6:
          set_(2, HL.value());
          break;

        case 0xD7:
          set_(2, A);
          break;

        case 0xD8:
          set_(3, BC.hi());
          break;

        case 0xD9:
          set_(3, BC.lo());
          break;

        case 0xDA:
          set_(3, DE.hi());
          break;

        case 0xDB:
          set_(3, DE.lo());
          break;

        case 0xDC:
          set_(3, HL.hi());
          break;

        case 0xDD:
          set_(3, HL.lo());
          break;

        case 0xDE:
          set_(3, HL.value());
          break;

        case 0xDF:
          set_(3, A);
          break;

        case 0xE0:
          set_(4, BC.hi());
          break;

        case 0xE1:
          set_(4, BC.lo());
          break;

        case 0xE2:
          set_(4, DE.hi());
          break;

        case 0xE3:
          set_(4, DE.lo());
          break;

        case 0xE4:
          set_(4, HL.hi());
          break;

        case 0xE5:
          set_(4, HL.lo());
          break;

        case 0xE6:
          set_(4, HL.value());
          break;

        case 0xE7:
          set_(4, A);
          break;

        case 0xE8:
          set_(5, BC.hi());
          break;

        case 0xE9:
          set_(5, BC.lo());
          break;

        case 0xEA:
          set_(5, DE.hi());
          break;

        case 0xEB:
          set_(5, DE.lo());
          break;

        case 0xEC:
          set_(5, HL.hi());
          break;

        case 0xED:
          set_(5, HL.lo());
          break;

        case 0xEE:
          set_(5, HL.value());
          break;

        case 0xEF:
          set_(5, A);
          break;

        case 0xF0:
          set_(6, BC.hi());
          break;

        case 0xF1:
          set_(6, BC.lo());
          break;

        case 0xF2:
          set_(6, DE.hi());
          break;

        case 0xF3:
          set_(6, DE.lo());
          break;

        case 0xF4:
          set_(6, HL.hi());
          break;

        case 0xF5:
          set_(6, HL.lo());
          break;

        case 0xF6:
          set_(6, HL.value());
          break;

        case 0xF7:
          set_(6, A);
          break;

        case 0xF8:
          set_(7, BC.hi());
          break;

        case 0xF9:
          set_(7, BC.lo());
          break;

        case 0xFA:
          set_(7, DE.hi());
          break;

        case 0xFB:
          set_(7, DE.lo());
          break;

        case 0xFC:
          set_(7, HL.hi());
          break;

        case 0xFD:
          set_(7, HL.lo());
          break;

        case 0xFE:
          set_(7, HL.value());
          break;

        case 0xFF:
          set_(7, A);
          break;

        default:
          break;
      }
      break;

    case 0xCC:
      call(cc::Z, n16(fetch_word()));
      break;

    case 0xCD:
      call(n16(fetch_word()));
      break;

    case 0xCE:
      adc(n8(fetch_byte()));
      break;

    case 0xCF:
      rst(1);
      break;

    case 0xD0:
      ret(cc::NC);
      break;

    case 0xD1:
      pop(DE);
      break;

    case 0xD2:
      jp(cc::NC, n16(fetch_word()));
      break;

    case 0xD4:
      call(cc::NC, n16(fetch_word()));
      break;

    case 0xD5:
      push(DE);
      break;

    case 0xD6:
      sub(n8(fetch_byte()));
      break;

    case 0xD7:
      rst(2);
      break;

    case 0xD8:
      ret(cc::C);
      break;

    case 0xD9:
      reti();
      break;

    case 0xDA:
      jp(cc::C, n16(fetch_word()));
      break;

    case 0xDC:
      call(cc::C, n16(fetch_word()));
      break;

    case 0xDE:
      sbc(n8(fetch_byte()));
      break;

    case 0xDF:
      rst(3);
      break;

    case 0xE0:
      ldio(0xFF00 + n8(fetch_byte()).value(), A);
      break;

    case 0xE1:
      pop(HL);
      break;

    case 0xE2:
      ldio(0xFF00 + BC.lo().value(), A);
      break;

    case 0xE5:
      push(HL);
      break;

    case 0xE6:
      and_(n8(fetch_byte()));
      break;

    case 0xE7:
      rst(4);
      break;

    case 0xE8:
      PC += 1;
      add(e8(m_bus.readBus(PC++)));
      break;

    case 0xE9:
      jp();
      break;

    case 0xEA:
      ld(n16(fetch_word()), tag{});
      break;

    case 0xEE:
      xor_(n8(fetch_byte()));
      break;

    case 0xEF:
      rst(5);
      break;

    case 0xF0:
      ldio(A, 0xFF00 + n8(fetch_byte()).value());
      break;

    case 0xF1:
      pop();
      break;

    case 0xF2:
      ldio(A, 0xFF00 + BC.lo().value());
      break;

    case 0xF3:
      di();
      break;

    case 0xF5:
      push();
      break;

    case 0xF6:
      or_(n8(fetch_byte()));
      break;

    case 0xF7:
      rst(6);
      break;

    case 0xF8:
      PC += 1;
      HL = SP + e8(m_bus.readBus(PC++)).value();
      m_clock.cycle(3);
      break;

    case 0xF9:
      SP = HL.value();
      m_clock.cycle(2);
      break;

    case 0xFA:
      ld(tag{}, n16(fetch_word()));
      break;

    case 0xFB:
      ei();
      break;

    case 0xFE:
      cp(n8(fetch_byte()));
      break;

    case 0xFF:
      rst(7);
      break;

    default:
      break;
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
  HL.value() + rr.value() > r16::max ? F.c(set) : F.c(reset);
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
  byte val = m_bus.readBus(uu);
  --val;
  m_bus.writeBus(uu, val);

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

  byte val = m_bus.readBus(uu);
  val == 0b0000'1111 ? F.h(set) : F.h(reset);
  ++val;
  m_bus.writeBus(uu, val);

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

  byte val = m_bus.readBus(uu);
  val = val & reset_bit_mask;

  m_bus.writeBus(uu, val);

  m_clock.cycle(4);
}

void cpu::set_(const uint8 pos, r8 &r) {
  uint8 set_bit_mask = (0b1 << pos);
  r = r.value() & set_bit_mask;

  m_clock.cycle(2);
}

void cpu::set_(const uint8 pos, const uint16 uu) {
  uint8 set_bit_mask = (0b1 << pos);

  byte val = m_bus.readBus(uu);
  val = val & set_bit_mask;

  m_bus.writeBus(uu, val);

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
  byte val = m_bus.readBus(uu);
  val = (val & 0b0000'1111) << 4 | (val & 0b1111'0000) >> 4;
  m_bus.writeBus(uu, val);

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

  byte val = m_bus.readBus(uu);
  val = (val << 1) | old_carry;
  m_bus.writeBus(uu, val);

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
  byte val = m_bus.readBus(uu);

  uint8 old_seventh_bit = val >> 7;
  val = (val << 1) | old_seventh_bit;
  m_bus.writeBus(uu, val);

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
  byte val = m_bus.readBus(uu);

  uint8 old_first = val & 0b0000'0001;
  uint8 carry = F.c() == set ? 1 : 0;

  val = (val >> 1) | (carry << 7);

  m_bus.writeBus(uu, val);

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
  byte val = m_bus.readBus(uu);
  uint8 old_first = val & 0b0000'0001;

  val = (val >> 1) | (old_first << 7);
  m_bus.writeBus(uu, val);

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
  byte val = m_bus.readBus(uu);
  uint8 old_seventh_bit = val >> 7;

  val = val << 1;
  m_bus.writeBus(uu, val);

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
  byte val = m_bus.readBus(uu);

  uint8 old_first_bit = val & 0b0000'0001;
  uint8 old_seventh_bit = val >> 7;

  val = (val >> 1) | (old_seventh_bit << 7);
  m_bus.writeBus(uu, val);

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
  byte val = m_bus.readBus(uu);

  uint8 old_first_bit = val & 0b0000'0001;

  val = val >> 1;
  m_bus.writeBus(uu, val);

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
  m_bus.writeBus(uu, r.value());

  m_clock.cycle(2);
}

void cpu::ld(r8 &r, const uint16 uu) {
  r = m_bus.readBus(uu);

  m_clock.cycle(2);
}

void cpu::ldi(r16 &rr, const r8 r) {
  m_bus.writeBus(rr.value(), r.value());
  rr++;

  m_clock.cycle(2);
}

void cpu::ldi(r8 &r, r16 &rr) {
  r = m_bus.readBus(rr.value());
  rr++;

  m_clock.cycle(2);
}

void cpu::ldd(r16 &rr, const r8 r) {
  m_bus.writeBus(rr.value(), r.value());
  rr--;

  m_clock.cycle(2);
}

void cpu::ldd(r8 &r, r16 &rr) {
  r = m_bus.readBus(rr.value());
  rr--;

  m_clock.cycle(2);
}

void cpu::ld(r16 &rr, const n16 nn) {
  rr = nn;

  m_clock.cycle(3);
}

void cpu::ld(const n16 nn, tag dummy) {
  m_bus.writeBus(nn.value(), A.value());

  m_clock.cycle(4);
}

void cpu::ld(tag dummy, const n16 nn) {
  A = m_bus.readBus(nn.value());

  m_clock.cycle(4);
}

void cpu::ldio(const uint16 nn, const r8 r) {
  m_bus.writeBus(nn, r.value());

  // Fix: if nn is 0xFF00 + C, then cycle 2 times
  m_clock.cycle(3);
}

void cpu::ldio(r8 &r, const uint16 nn) {
  r = m_bus.readBus(nn);

  // Fix: if nn is 0xFF00 + C, then cycle 2 times
  m_clock.cycle(3);
}

// come back to here
void cpu::call(n16 nn) {

  m_bus.writeBus(SP - 1, PC & r16::reset_lower >> 8);
  m_bus.writeBus(SP - 2, PC & r16::reset_upper);

  PC = nn.value();

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
  PC = PC + e.value();

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
    byte lo = m_bus.readBus(SP);
    byte hi = m_bus.readBus(SP+1);
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
  byte lo = m_bus.readBus(SP);
  byte hi = m_bus.readBus(SP + 1);
  PC = hi << 8 | lo;

  SP += 2;

  m_clock.cycle(4);
}

void cpu::rst(const uint8 u) {
  m_bus.writeBus(SP - 1, PC & r16::reset_lower >> 8);
  m_bus.writeBus(SP - 2, PC & r16::reset_upper);
  SP -= 2;

  PC = (PC & r16::reset_upper) | rst_vec.at(u);

  m_clock.cycle(4);
}

void cpu::pop() {
  F = m_bus.readBus(SP);
  A = m_bus.readBus(SP + 1);
  SP += 2;

  m_clock.cycle(3);
}

void cpu::pop(r16 &rr) {
  rr.lo() = m_bus.readBus(SP);
  rr.hi() = m_bus.readBus(SP + 1);
  SP += 2;

  m_clock.cycle(3);
}

void cpu::push() {
  m_bus.writeBus(SP - 1, A.value());
  m_bus.writeBus(SP - 2, F.value());
  SP -= 2;

  m_clock.cycle(4);
}

void cpu::push(r16 &rr) {
  m_bus.writeBus(SP - 1, rr.hi().value());
  m_bus.writeBus(SP - 2, rr.lo().value());
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

void cpu::daa() {

  // daa instruction, taken from:
  // http://forums.nesdev.com/viewtopic.php?f=20&t=15944
  if (!F.n()) {
    if (F.c() || A.value() > 0x99) {
      A = A.value() + 0x60;
      F.c(set);
    }
    if (F.h() || (A.value() & 0x0F) > 0x09) {
      A = A.value() + 0x6;
    }
  } else {
    if (F.c()) {
      A = A.value() - 0x60;
    }
    if (F.h()) {
      A = A.value() - 0x6;
    }
  }

  A == 0 ? F.z(set) : F.z(reset);
  F.h(reset);

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

void cpu::halt() {
  // implement this
}

void cpu::nop() {
  //
  m_clock.cycle(1);
}

void cpu::scf() {
  F.n(reset);
  F.h(reset);
  F.c(set);

  m_clock.cycle(1);
}

void cpu::stop() {
  // implement this.
}
} // namespace tmbl
