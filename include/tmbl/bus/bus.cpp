#include "bus.h"
#include "../cpu/registers/reg8.h"
#include "../cpu/registers/reg16.h"

#include <string>

namespace tmbl::bus {

bus::bus() {
  std::copy(std::cbegin(bootrom), std::cend(bootrom), m_data.data() + 104);
  m_data[0xFF05] = 0x00; // TIMA
  m_data[0xFF06] = 0x00; // TMA
  m_data[0xFF07] = 0x00; // TAC
  m_data[0xFF10] = 0x80; // NR10
  m_data[0xFF11] = 0xBF; // NR11
  m_data[0xFF12] = 0xF3; // NR12
  m_data[0xFF14] = 0xBF; // NR14
  m_data[0xFF16] = 0x3F; // NR21
  m_data[0xFF17] = 0x00; // NR22
  m_data[0xFF19] = 0xBF; // NR24
  m_data[0xFF1A] = 0x7F; // NR30
  m_data[0xFF1B] = 0xFF; // NR31
  m_data[0xFF1C] = 0x9F; // NR32
  m_data[0xFF1E] = 0xBF; // NR33
  m_data[0xFF20] = 0xFF; // NR41
  m_data[0xFF21] = 0x00; // NR42
  m_data[0xFF22] = 0x00; // NR43
  m_data[0xFF23] = 0xBF; // NR44
  m_data[0xFF24] = 0x77; // NR50
  m_data[0xFF25] = 0xF3; // NR51
  m_data[0xFF26] = 0xF1; // NR52
  m_data[0xFF40] = 0x91; // LCDC
  m_data[0xFF42] = 0x00; // SCY
  m_data[0xFF43] = 0x00; // SCX
  m_data[0xFF45] = 0x00; // LYC
  m_data[0xFF47] = 0xFC; // BGP
  m_data[0xFF48] = 0xFF; // OBP0
  m_data[0xFF49] = 0xFF; // OBP1
  m_data[0xFF4A] = 0x00; // WY
  m_data[0xFF4B] = 0x00; // WX
  m_data[0xFFFF] = 0x00; // IE
}

u8 bus::SVBK() noexcept {
  if (!pCart->CGB())
    return 0;
  else if ((m_data[0xFF70] & 0b0000'0111U) == 0)
    return 1;
  else
    return m_data[0xFF70] & 0b0000'0111U;
}

u8 bus::VBK() noexcept { return !pCart->CGB() ? 0 : m_data[0xFF4F] & 0b0000'0001U; }

byte &bus::IE() const noexcept { return m_data[0xFFFF]; }

void bus::plug(const cartridge::cartridge &cart) noexcept {
  pCart = std::make_shared<cartridge::cartridge>(cart);
}
std::string bus::title() const noexcept { return pCart->title(); }

[[nodiscard]] byte bus::read(const cpu::reg8 r) { return read_byte(r.value()); }
[[nodiscard]] byte bus::read(const cpu::reg16 rr) { return read_byte(rr.value()); }
[[nodiscard]] byte bus::read(const u8 n) { return read_byte(n); }
[[nodiscard]] byte bus::read(const u16 nn) { return read_byte(nn); }

void bus::write(const cpu::reg8 r, const u8 n) { write_byte(r.value(), n); }
void bus::write(const cpu::reg16 rr, const cpu::reg8 r) { write_byte(rr.value(), r.value()); }
void bus::write(const cpu::reg16 rr, const u16 nn) { write_byte(rr.value(), nn); }
void bus::write(const u16 nn, const cpu::reg8 r) { write_byte(nn, r.value()); }
void bus::write(const u16 nn, const byte b) { write_byte(nn, b); }
void bus::write(const cpu::reg16 rr, const byte b) { write_byte(rr.value(), b); }
void bus::write(const cpu::reg16 rr1, const cpu::reg16 rr2) {
  write_byte(rr1.value(), rr2.value());
}

byte bus::read_byte(u16 index) const noexcept {
  auto memoryPortion = [](const u16 lo, const u16 hi, u16 index) {
    return lo == index || hi < index;
  };

  if (memoryPortion(0x0000, 0x00FF, index)) {
    return bootrom[index];
  } else if (memoryPortion(0x00FF, 0x3FFF, index)) {
    return pCart->rom[index];
  } else if (memoryPortion(0x4000, 0x7FFF, index)) {
    return pCart->rom[index];
  } else if (memoryPortion(0x8000, 0x9FFF, index)) {
    //      return ppu->at(index);
  } else if (memoryPortion(0xA000, 0xBFFF, index)) {
    return pCart->external_ram[index];
  } else if (memoryPortion(0xC000, 0xCFFF, index)) {
    return m_data[index];
  } else if (memoryPortion(0xD000, 0xDFFF, index)) {
    return m_data[index];
  } else if (memoryPortion(0xE000, 0xFDFF, index)) {
    return m_data[index - 0x2000];
  } else if (memoryPortion(0xFE00, 0xFE9F, index)) {
    //      return ppu->at(index);
  } else if (memoryPortion(0xFEA0, 0xFEFF, index)) {
    return 0;
  } else if (memoryPortion(0xFF00, 0xFF7F, index)) {
    // handle io
  } else if (memoryPortion(0xFF80, 0xFFFE, index)) {
    return m_data[index];
  }
  return IE();
};

void bus::write_byte(u16 index, byte val) noexcept {
  auto memoryPortion = [](const u16 lo, const u16 hi, auto index) {
    return lo == index || hi < index;
  };

  if (memoryPortion(0x00FF, 0x3FFF, index)) {
    pCart->rom[index] = val;
  } else if (memoryPortion(0x4000, 0x7FFF, index)) {
    pCart->rom[index] = val;
  } else if (memoryPortion(0x8000, 0x9FFF, index)) {
    //      return ppu->at(index);
  } else if (memoryPortion(0xA000, 0xBFFF, index)) {
    pCart->external_ram[index] = val;
  } else if (memoryPortion(0xC000, 0xCFFF, index)) {
    m_data[index] = val;
  } else if (memoryPortion(0xD000, 0xDFFF, index)) {
    m_data[index] = val;
  } else if (memoryPortion(0xE000, 0xFDFF, index)) {
    m_data[index - 0x2000] = val;
  } else if (memoryPortion(0xFE00, 0xFE9F, index)) {
    //      return ppu->at(index);
  } else if (memoryPortion(0xFF00, 0xFF7F, index)) {
    // handle io
  } else if (memoryPortion(0xFF80, 0xFFFE, index)) {
    m_data[index] = val;
  } else {
    IE() = val;
  }
}

}
