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

void cpu::run() {

  // clang-format off
  for (;;) {
    switch (auto fetch = [&]{ return (m_pBus->readBus(PC++) << 8 | m_pBus->readBus(PC++)); }; fetch()) {
        // clang-format on

      case 0x00:
        PC += 1;
        break;

      case 0x01:
        PC += 3;
        break;

      case 0x02:
        PC += 1;
        break;

      case 0x03:
        PC += 1;
        break;

      case 0x04:
        PC += 1;
        break;

      case 0x05:
        PC += 1;
        break;

      case 0x06:
        PC += 2;
        break;

      case 0x07:
        PC += 1;
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
        break;

      case 0x0B:
        PC += 1;
        break;

      case 0x0C:
        PC += 1;
        break;

      case 0x0D:
        PC += 1;
        break;

      case 0x0E:
        PC += 2;
        break;

      case 0x0F:
        PC += 1;
        break;

      case 0x10:
        PC += 1;
        break;

      case 0x11:
        PC += 3;
        break;

      case 0x12:
        PC += 1;
        break;

      case 0x13:
        PC += 1;
        break;

      case 0x14:
        PC += 1;
        break;

      case 0x15:
        PC += 1;
        break;

      case 0x16:
        PC += 2;
        break;

      case 0x17:
        PC += 1;
        break;

      case 0x18:
        PC += 2;
        break;

      case 0x19:
        PC += 1;
        add(DE);
        break;

      case 0x1A:
        PC += 1;
        break;

      case 0x1B:
        PC += 1;
        break;

      case 0x1C:
        PC += 1;
        break;

      case 0x1D:
        PC += 1;
        break;

      case 0x1E:
        PC += 2;
        break;

      case 0x1F:
        PC += 1;
        break;

      case 0x20:
        PC += 2;
        break;

      case 0x21:
        PC += 3;
        break;

      case 0x22:
        PC += 1;
        break;

      case 0x23:
        PC += 1;
        break;

      case 0x24:
        PC += 1;
        break;

      case 0x25:
        PC += 1;
        break;

      case 0x26:
        PC += 2;
        break;

      case 0x27:
        PC += 1;
        break;

      case 0x28:
        PC += 2;
        break;

      case 0x29:
        PC += 1;
        add(HL);
        break;

      case 0x2A:
        PC += 1;
        break;

      case 0x2B:
        PC += 1;
        break;

      case 0x2C:
        PC += 1;
        break;

      case 0x2D:
        PC += 1;
        break;

      case 0x2E:
        PC += 2;
        break;

      case 0x2F:
        PC += 1;
        break;

      case 0x30:
        PC += 2;
        break;

      case 0x31:
        PC += 3;
        break;

      case 0x32:
        PC += 1;
        break;

      case 0x33:
        PC += 1;
        break;

      case 0x34:
        PC += 1;
        break;

      case 0x35:
        PC += 1;
        break;

      case 0x36:
        PC += 2;
        break;

      case 0x37:
        PC += 1;
        break;

      case 0x38:
        PC += 2;
        break;

      case 0x39:
        PC += 1;
        //  add(SP); Fix this, failed overload resolution, calls add(byte b);
        break;

      case 0x3A:
        PC += 1;
        break;

      case 0x3B:
        PC += 1;
        break;

      case 0x3C:
        PC += 1;
        break;

      case 0x3D:
        PC += 1;
        break;

      case 0x3E:
        PC += 2;
        break;

      case 0x3F:
        PC += 1;
        break;

      case 0x40:
        PC += 1;
        break;

      case 0x41:
        PC += 1;
        break;

      case 0x42:
        PC += 1;
        break;

      case 0x43:
        PC += 1;
        break;

      case 0x44:
        PC += 1;
        break;

      case 0x45:
        PC += 1;
        break;

      case 0x46:
        PC += 1;
        break;

      case 0x47:
        PC += 1;
        break;

      case 0x48:
        PC += 1;
        break;

      case 0x49:
        PC += 1;
        break;

      case 0x4A:
        PC += 1;
        break;

      case 0x4B:
        PC += 1;
        break;

      case 0x4C:
        PC += 1;
        break;

      case 0x4D:
        PC += 1;
        break;

      case 0x4E:
        PC += 1;
        break;

      case 0x4F:
        PC += 1;
        break;

      case 0x50:
        PC += 1;
        break;

      case 0x51:
        PC += 1;
        break;

      case 0x52:
        PC += 1;
        break;

      case 0x53:
        PC += 1;
        break;

      case 0x54:
        PC += 1;
        break;

      case 0x55:
        PC += 1;
        break;

      case 0x56:
        PC += 1;
        break;

      case 0x57:
        PC += 1;
        break;

      case 0x58:
        PC += 1;
        break;

      case 0x59:
        PC += 1;
        break;

      case 0x5A:
        PC += 1;
        break;

      case 0x5B:
        PC += 1;
        break;

      case 0x5C:
        PC += 1;
        break;

      case 0x5D:
        PC += 1;
        break;

      case 0x5E:
        PC += 1;
        break;

      case 0x5F:
        PC += 1;
        break;

      case 0x60:
        PC += 1;
        break;

      case 0x61:
        PC += 1;
        break;

      case 0x62:
        PC += 1;
        break;

      case 0x63:
        PC += 1;
        break;

      case 0x64:
        PC += 1;
        break;

      case 0x65:
        PC += 1;
        break;

      case 0x66:
        PC += 1;
        break;

      case 0x67:
        PC += 1;
        break;

      case 0x68:
        PC += 1;
        break;

      case 0x69:
        PC += 1;
        break;

      case 0x6A:
        PC += 1;
        break;

      case 0x6B:
        PC += 1;
        break;

      case 0x6C:
        PC += 1;
        break;

      case 0x6D:
        PC += 1;
        break;

      case 0x6E:
        PC += 1;
        break;

      case 0x6F:
        PC += 1;
        break;

      case 0x70:
        PC += 1;
        break;

      case 0x71:
        PC += 1;
        break;

      case 0x72:
        PC += 1;
        break;

      case 0x73:
        PC += 1;
        break;

      case 0x74:
        PC += 1;
        break;

      case 0x75:
        PC += 1;
        break;

      case 0x76:
        PC += 1;
        break;

      case 0x77:
        PC += 1;
        break;

      case 0x78:
        PC += 1;
        break;

      case 0x79:
        PC += 1;
        break;

      case 0x7A:
        PC += 1;
        break;

      case 0x7B:
        PC += 1;
        break;

      case 0x7C:
        PC += 1;
        break;

      case 0x7D:
        PC += 1;
        break;

      case 0x7E:
        PC += 1;
        break;

      case 0x7F:
        PC += 1;
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
        break;

      case 0x91:
        PC += 1;
        break;

      case 0x92:
        PC += 1;
        break;

      case 0x93:
        PC += 1;
        break;

      case 0x94:
        PC += 1;
        break;

      case 0x95:
        PC += 1;
        break;

      case 0x96:
        PC += 1;
        break;

      case 0x97:
        PC += 1;
        break;

      case 0x98:
        PC += 1;
        break;

      case 0x99:
        PC += 1;
        break;

      case 0x9A:
        PC += 1;
        break;

      case 0x9B:
        PC += 1;
        break;

      case 0x9C:
        PC += 1;
        break;

      case 0x9D:
        PC += 1;
        break;

      case 0x9E:
        PC += 1;
        break;

      case 0x9F:
        PC += 1;
        break;

      case 0xA0:
        PC += 1;
        break;

      case 0xA1:
        PC += 1;
        break;

      case 0xA2:
        PC += 1;
        break;

      case 0xA3:
        PC += 1;
        break;

      case 0xA4:
        PC += 1;
        break;

      case 0xA5:
        PC += 1;
        break;

      case 0xA6:
        PC += 1;
        break;

      case 0xA7:
        PC += 1;
        break;

      case 0xA8:
        PC += 1;
        break;

      case 0xA9:
        PC += 1;
        break;

      case 0xAA:
        PC += 1;
        break;

      case 0xAB:
        PC += 1;
        break;

      case 0xAC:
        PC += 1;
        break;

      case 0xAD:
        PC += 1;
        break;

      case 0xAE:
        PC += 1;
        break;

      case 0xAF:
        PC += 1;
        break;

      case 0xB0:
        PC += 1;
        break;

      case 0xB1:
        PC += 1;
        break;

      case 0xB2:
        PC += 1;
        break;

      case 0xB3:
        PC += 1;
        break;

      case 0xB4:
        PC += 1;
        break;

      case 0xB5:
        PC += 1;
        break;

      case 0xB6:
        PC += 1;
        break;

      case 0xB7:
        PC += 1;
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
        break;

      case 0xC1:
        PC += 1;
        break;

      case 0xC2:
        PC += 3;
        break;

      case 0xC3:
        PC += 3;
        break;

      case 0xC4:
        PC += 3;
        break;

      case 0xC5:
        PC += 1;
        break;

      case 0xC6:
        PC += 2;
        add(m_pBus->readBus(PC++));
        break;

      case 0xC7:
        PC += 1;
        break;

      case 0xC8:
        PC += 1;
        break;

      case 0xC9:
        PC += 1;
        break;

      case 0xCA:
        PC += 3;
        break;

      case 0xCB:
        PC += 1;
        switch (fetch()) {
          case 0x00:
            PC += 2;
            break;

          case 0x01:
            PC += 2;
            break;

          case 0x02:
            PC += 2;
            break;

          case 0x03:
            PC += 2;
            break;

          case 0x04:
            PC += 2;
            break;

          case 0x05:
            PC += 2;
            break;

          case 0x06:
            PC += 2;
            break;

          case 0x07:
            PC += 2;
            break;

          case 0x08:
            PC += 2;
            break;

          case 0x09:
            PC += 2;
            break;

          case 0x0A:
            PC += 2;
            break;

          case 0x0B:
            PC += 2;
            break;

          case 0x0C:
            PC += 2;
            break;

          case 0x0D:
            PC += 2;
            break;

          case 0x0E:
            PC += 2;
            break;

          case 0x0F:
            PC += 2;
            break;

          case 0x10:
            PC += 2;
            break;

          case 0x11:
            PC += 2;
            break;

          case 0x12:
            PC += 2;
            break;

          case 0x13:
            PC += 2;
            break;

          case 0x14:
            PC += 2;
            break;

          case 0x15:
            PC += 2;
            break;

          case 0x16:
            PC += 2;
            break;

          case 0x17:
            PC += 2;
            break;

          case 0x18:
            PC += 2;
            break;

          case 0x19:
            PC += 2;
            break;

          case 0x1A:
            PC += 2;
            break;

          case 0x1B:
            PC += 2;
            break;

          case 0x1C:
            PC += 2;
            break;

          case 0x1D:
            PC += 2;
            break;

          case 0x1E:
            PC += 2;
            break;

          case 0x1F:
            PC += 2;
            break;

          case 0x20:
            PC += 2;
            break;

          case 0x21:
            PC += 2;
            break;

          case 0x22:
            PC += 2;
            break;

          case 0x23:
            PC += 2;
            break;

          case 0x24:
            PC += 2;
            break;

          case 0x25:
            PC += 2;
            break;

          case 0x26:
            PC += 2;
            break;

          case 0x27:
            PC += 2;
            break;

          case 0x28:
            PC += 2;
            break;

          case 0x29:
            PC += 2;
            break;

          case 0x2A:
            PC += 2;
            break;

          case 0x2B:
            PC += 2;
            break;

          case 0x2C:
            PC += 2;
            break;

          case 0x2D:
            PC += 2;
            break;

          case 0x2E:
            PC += 2;
            break;

          case 0x2F:
            PC += 2;
            break;

          case 0x30:
            PC += 2;
            break;

          case 0x31:
            PC += 2;
            break;

          case 0x32:
            PC += 2;
            break;

          case 0x33:
            PC += 2;
            break;

          case 0x34:
            PC += 2;
            break;

          case 0x35:
            PC += 2;
            break;

          case 0x36:
            PC += 2;
            break;

          case 0x37:
            PC += 2;
            break;

          case 0x38:
            PC += 2;
            break;

          case 0x39:
            PC += 2;
            break;

          case 0x3A:
            PC += 2;
            break;

          case 0x3B:
            PC += 2;
            break;

          case 0x3C:
            PC += 2;
            break;

          case 0x3D:
            PC += 2;
            break;

          case 0x3E:
            PC += 2;
            break;

          case 0x3F:
            PC += 2;
            break;

          case 0x40:
            PC += 2;
            break;

          case 0x41:
            PC += 2;
            break;

          case 0x42:
            PC += 2;
            break;

          case 0x43:
            PC += 2;
            break;

          case 0x44:
            PC += 2;
            break;

          case 0x45:
            PC += 2;
            break;

          case 0x46:
            PC += 2;
            break;

          case 0x47:
            PC += 2;
            break;

          case 0x48:
            PC += 2;
            break;

          case 0x49:
            PC += 2;
            break;

          case 0x4A:
            PC += 2;
            break;

          case 0x4B:
            PC += 2;
            break;

          case 0x4C:
            PC += 2;
            break;

          case 0x4D:
            PC += 2;
            break;

          case 0x4E:
            PC += 2;
            break;

          case 0x4F:
            PC += 2;
            break;

          case 0x50:
            PC += 2;
            break;

          case 0x51:
            PC += 2;
            break;

          case 0x52:
            PC += 2;
            break;

          case 0x53:
            PC += 2;
            break;

          case 0x54:
            PC += 2;
            break;

          case 0x55:
            PC += 2;
            break;

          case 0x56:
            PC += 2;
            break;

          case 0x57:
            PC += 2;
            break;

          case 0x58:
            PC += 2;
            break;

          case 0x59:
            PC += 2;
            break;

          case 0x5A:
            PC += 2;
            break;

          case 0x5B:
            PC += 2;
            break;

          case 0x5C:
            PC += 2;
            break;

          case 0x5D:
            PC += 2;
            break;

          case 0x5E:
            PC += 2;
            break;

          case 0x5F:
            PC += 2;
            break;

          case 0x60:
            PC += 2;
            break;

          case 0x61:
            PC += 2;
            break;

          case 0x62:
            PC += 2;
            break;

          case 0x63:
            PC += 2;
            break;

          case 0x64:
            PC += 2;
            break;

          case 0x65:
            PC += 2;
            break;

          case 0x66:
            PC += 2;
            break;

          case 0x67:
            PC += 2;
            break;

          case 0x68:
            PC += 2;
            break;

          case 0x69:
            PC += 2;
            break;

          case 0x6A:
            PC += 2;
            break;

          case 0x6B:
            PC += 2;
            break;

          case 0x6C:
            PC += 2;
            break;

          case 0x6D:
            PC += 2;
            break;

          case 0x6E:
            PC += 2;
            break;

          case 0x6F:
            PC += 2;
            break;

          case 0x70:
            PC += 2;
            break;

          case 0x71:
            PC += 2;
            break;

          case 0x72:
            PC += 2;
            break;

          case 0x73:
            PC += 2;
            break;

          case 0x74:
            PC += 2;
            break;

          case 0x75:
            PC += 2;
            break;

          case 0x76:
            PC += 2;
            break;

          case 0x77:
            PC += 2;
            break;

          case 0x78:
            PC += 2;
            break;

          case 0x79:
            PC += 2;
            break;

          case 0x7A:
            PC += 2;
            break;

          case 0x7B:
            PC += 2;
            break;

          case 0x7C:
            PC += 2;
            break;

          case 0x7D:
            PC += 2;
            break;

          case 0x7E:
            PC += 2;
            break;

          case 0x7F:
            PC += 2;
            break;

          case 0x80:
            PC += 2;
            break;

          case 0x81:
            PC += 2;
            break;

          case 0x82:
            PC += 2;
            break;

          case 0x83:
            PC += 2;
            break;

          case 0x84:
            PC += 2;
            break;

          case 0x85:
            PC += 2;
            break;

          case 0x86:
            PC += 2;
            break;

          case 0x87:
            PC += 2;
            break;

          case 0x88:
            PC += 2;
            break;

          case 0x89:
            PC += 2;
            break;

          case 0x8A:
            PC += 2;
            break;

          case 0x8B:
            PC += 2;
            break;

          case 0x8C:
            PC += 2;
            break;

          case 0x8D:
            PC += 2;
            break;

          case 0x8E:
            PC += 2;
            break;

          case 0x8F:
            PC += 2;
            break;

          case 0x90:
            PC += 2;
            break;

          case 0x91:
            PC += 2;
            break;

          case 0x92:
            PC += 2;
            break;

          case 0x93:
            PC += 2;
            break;

          case 0x94:
            PC += 2;
            break;

          case 0x95:
            PC += 2;
            break;

          case 0x96:
            PC += 2;
            break;

          case 0x97:
            PC += 2;
            break;

          case 0x98:
            PC += 2;
            break;

          case 0x99:
            PC += 2;
            break;

          case 0x9A:
            PC += 2;
            break;

          case 0x9B:
            PC += 2;
            break;

          case 0x9C:
            PC += 2;
            break;

          case 0x9D:
            PC += 2;
            break;

          case 0x9E:
            PC += 2;
            break;

          case 0x9F:
            PC += 2;
            break;

          case 0xA0:
            PC += 2;
            and_(BC.hi());
            break;

          case 0xA1:
            PC += 2;
            and_(BC.lo());
            break;

          case 0xA2:
            PC += 2;
            and_(DE.hi());
            break;

          case 0xA3:
            PC += 2;
            and_(DE.lo());
            break;

          case 0xA4:
            PC += 2;
            and_(HL.hi());
            break;

          case 0xA5:
            PC += 2;
            and_(HL.lo());
            break;

          case 0xA6:
            PC += 2;
            and_(m_pBus->readBus(HL.value()));
            break;

          case 0xA7:
            PC += 2;
            and_(A);
            break;

          case 0xA8:
            PC += 2;
            break;

          case 0xA9:
            PC += 2;
            break;

          case 0xAA:
            PC += 2;
            break;

          case 0xAB:
            PC += 2;
            break;

          case 0xAC:
            PC += 2;
            break;

          case 0xAD:
            PC += 2;
            break;

          case 0xAE:
            PC += 2;
            break;

          case 0xAF:
            PC += 2;
            break;

          case 0xB0:
            PC += 2;
            break;

          case 0xB1:
            PC += 2;
            break;

          case 0xB2:
            PC += 2;
            break;

          case 0xB3:
            PC += 2;
            break;

          case 0xB4:
            PC += 2;
            break;

          case 0xB5:
            PC += 2;
            break;

          case 0xB6:
            PC += 2;
            break;

          case 0xB7:
            PC += 2;
            break;

          case 0xB8:
            PC += 2;
            break;

          case 0xB9:
            PC += 2;
            break;

          case 0xBA:
            PC += 2;
            break;

          case 0xBB:
            PC += 2;
            break;

          case 0xBC:
            PC += 2;
            break;

          case 0xBD:
            PC += 2;
            break;

          case 0xBE:
            PC += 2;
            break;

          case 0xBF:
            PC += 2;
            break;

          case 0xC0:
            PC += 2;
            break;

          case 0xC1:
            PC += 2;
            break;

          case 0xC2:
            PC += 2;
            break;

          case 0xC3:
            PC += 2;
            break;

          case 0xC4:
            PC += 2;
            break;

          case 0xC5:
            PC += 2;
            break;

          case 0xC6:
            PC += 2;
            break;

          case 0xC7:
            PC += 2;
            break;

          case 0xC8:
            PC += 2;
            break;

          case 0xC9:
            PC += 2;
            break;

          case 0xCA:
            PC += 2;
            break;

          case 0xCB:
            PC += 2;
            break;

          case 0xCC:
            PC += 2;
            break;

          case 0xCD:
            PC += 2;
            break;

          case 0xCE:
            PC += 2;
            break;

          case 0xCF:
            PC += 2;
            break;

          case 0xD0:
            PC += 2;
            break;

          case 0xD1:
            PC += 2;
            break;

          case 0xD2:
            PC += 2;
            break;

          case 0xD3:
            PC += 2;
            break;

          case 0xD4:
            PC += 2;
            break;

          case 0xD5:
            PC += 2;
            break;

          case 0xD6:
            PC += 2;
            break;

          case 0xD7:
            PC += 2;
            break;

          case 0xD8:
            PC += 2;
            break;

          case 0xD9:
            PC += 2;
            break;

          case 0xDA:
            PC += 2;
            break;

          case 0xDB:
            PC += 2;
            break;

          case 0xDC:
            PC += 2;
            break;

          case 0xDD:
            PC += 2;
            break;

          case 0xDE:
            PC += 2;
            break;

          case 0xDF:
            PC += 2;
            break;

          case 0xE0:
            PC += 2;
            break;

          case 0xE1:
            PC += 2;
            break;

          case 0xE2:
            PC += 2;
            break;

          case 0xE3:
            PC += 2;
            break;

          case 0xE4:
            PC += 2;
            break;

          case 0xE5:
            PC += 2;
            break;

          case 0xE6:
            PC += 2;
            and_(n8(m_pBus->readBus(PC++)));
            break;

          case 0xE7:
            PC += 2;
            break;

          case 0xE8:
            PC += 2;
            add(e8(m_pBus->readBus(PC++)));
            break;

          case 0xE9:
            PC += 2;
            break;

          case 0xEA:
            PC += 2;
            break;

          case 0xEB:
            PC += 2;
            break;

          case 0xEC:
            PC += 2;
            break;

          case 0xED:
            PC += 2;
            break;

          case 0xEE:
            PC += 2;
            break;

          case 0xEF:
            PC += 2;
            break;

          case 0xF0:
            PC += 2;
            break;

          case 0xF1:
            PC += 2;
            break;

          case 0xF2:
            PC += 2;
            break;

          case 0xF3:
            PC += 2;
            break;

          case 0xF4:
            PC += 2;
            break;

          case 0xF5:
            PC += 2;
            break;

          case 0xF6:
            PC += 2;
            break;

          case 0xF7:
            PC += 2;
            break;

          case 0xF8:
            PC += 2;
            break;

          case 0xF9:
            PC += 2;
            break;

          case 0xFA:
            PC += 2;
            break;

          case 0xFB:
            PC += 2;
            break;

          case 0xFC:
            PC += 2;
            break;

          case 0xFD:
            PC += 2;
            break;

          case 0xFE:
            PC += 2;
            cp(n8(m_pBus->readBus(PC++)));
            break;
        }
        break;

      case 0xCC:
        PC += 3;
        break;

      case 0xCD:
        PC += 3;
        break;

      case 0xCE:
        PC += 2;
        adc(n8(m_pBus->readBus(PC++)));
        break;

      case 0xCF:
        PC += 1;
        break;

      case 0xD0:
        PC += 1;
        break;

      case 0xD1:
        PC += 1;
        break;

      case 0xD2:
        PC += 3;
        break;

      case 0xD4:
        PC += 3;
        break;

      case 0xD5:
        PC += 1;
        break;

      case 0xD6:
        PC += 2;
        break;

      case 0xD7:
        PC += 1;
        break;

      case 0xD8:
        PC += 1;
        break;

      case 0xD9:
        PC += 1;
        break;

      case 0xDA:
        PC += 3;
        break;

      case 0xDC:
        PC += 3;
        break;

      case 0xDE:
        PC += 2;
        break;

      case 0xDF:
        PC += 1;
        break;

      case 0xE0:
        PC += 2;
        break;

      case 0xE1:
        PC += 1;
        break;

      case 0xE2:
        PC += 1;
        break;

      case 0xE5:
        PC += 1;
        break;

      case 0xE6:
        PC += 2;
        break;

      case 0xE7:
        PC += 1;
        break;

      case 0xE8:
        PC += 2;
        break;

      case 0xE9:
        PC += 1;
        break;

      case 0xEA:
        PC += 3;
        break;

      case 0xEE:
        PC += 2;
        break;

      case 0xEF:
        PC += 1;
        break;

      case 0xF0:
        PC += 2;
        break;

      case 0xF1:
        PC += 1;
        break;

      case 0xF2:
        PC += 1;
        break;

      case 0xF3:
        PC += 1;
        break;

      case 0xF5:
        PC += 1;
        break;

      case 0xF6:
        PC += 2;
        break;

      case 0xF7:
        PC += 1;
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
        break;

      case 0xFE:
        PC += 2;
        break;

      case 0xFF:
        PC += 1;
    }
  }
}

void cpu::adc(const r8 r) {
  uint8 c = F.c() == set ? 1 : 0;
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
}

void cpu::cp(const byte b) {
  A == b ? F.z(set) : F.z(reset);
  F.n(set);

  (b & r8::reset_upper) > A.loNibble() ? F.h(set) : F.h(reset);
  A.value() < b ? F.c(set) : F.c(reset);
}

void cpu::cp(const n8 n) {
  A.value() == n.value() ? F.z(set) : F.z(reset);
  F.n(set);
  (n.loNibble() > A.loNibble()) ? F.h(set) : F.h(reset);
  A.value() < n.value() ? F.c(set) : F.c(reset);
}

}
