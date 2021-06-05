#include "tmbl/bus/bus.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/config.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/io/registers.h"
#include "tmbl/memory_map.h"
#include "tmbl/ppu/ppu.h"

#include <iostream>

namespace tmbl {

bus::bus(cartridge &pCart, registers &pRegs, interrupts &pIntr, builtin &pBuiltin, ppu &pPPU)
    : m_cart(pCart), m_regs(pRegs), m_pintr(pIntr), m_builtin(pBuiltin), m_PPU(pPPU) {

  writeBus(0xFF05, 0x00); //  TIMA
  writeBus(0xFF06, 0x00); //  TMA
  writeBus(0xFF07, 0x00); //  TAC
  writeBus(0xFF10, 0x80); //  NR10
  writeBus(0xFF11, 0xBF); //  NR11
  writeBus(0xFF12, 0xF3); //  NR12
  writeBus(0xFF14, 0xBF); //  NR14
  writeBus(0xFF16, 0x3F); //  NR21
  writeBus(0xFF17, 0x00); //  NR22
  writeBus(0xFF19, 0xBF); //  NR24
  writeBus(0xFF1A, 0x7F); //  NR30
  writeBus(0xFF1B, 0xFF); //  NR31
  writeBus(0xFF1C, 0x9F); //  NR32
  writeBus(0xFF1E, 0xBF); //  NR34
  writeBus(0xFF20, 0xFF); //  NR41
  writeBus(0xFF21, 0x00); //  NR42
  writeBus(0xFF22, 0x00); //  NR43
  writeBus(0xFF23, 0xBF); //  NR44
  writeBus(0xFF24, 0x77); //  NR50
  writeBus(0xFF25, 0xF3); //  NR51
  writeBus(0xFF26, 0xF1); //  NR52
  writeBus(0xFF40, 0x91); //  LCDC
  writeBus(0xFF42, 0x00); //  SCY
  writeBus(0xFF43, 0x00); //  SCX
  writeBus(0xFF45, 0x00); //  LYC
  writeBus(0xFF47, 0xFC); //  BGP
  writeBus(0xFF48, 0xFF); //  OBP0
  writeBus(0xFF49, 0xFF); //  OBP1
  writeBus(0xFF4A, 0x00); //  WY
  writeBus(0xFF4B, 0x00); //  WX
  writeBus(0xFFFF, 0x00); //  IE
}

byte bus::readBus(const std::size_t index) const noexcept {
  if (index >= memory::rom0 && index <= memory::romx_end) {
    return m_cart.readROM(index - memory::rom0);
  }

  else if (index >= memory::vram && index <= memory::vram_end) {
    return m_PPU.readVRAM(index - memory::vram);
  }

  else if (index >= memory::xram && index <= memory::xram_end) {
    return m_cart.readXRAM(index - memory::xram);
  }

  else if (index >= memory::wram0 && index <= memory::wramx_end) {
    return m_builtin.readWRAM(index - memory::wram0);
  }

  else if (index >= memory::echo && index <= memory::echo_end) {
    return m_builtin.readEcho(index - memory::echo);
  }

  else if (index >= memory::oam && index <= memory::oam_end) {
    return m_PPU.readOAM(index - memory::oam);
  }

  else if (index >= memory::noUsable && index <= memory::noUsable_end) {
    return 0xFF; // read from the no usable area
  }

  else if (index >= memory::io && index <= memory::io_end) {
    if (index == 0xFF0F) {
      return m_pintr.read(index);
    }
    return m_regs.read(index - memory::io);
  }

  else if (index >= memory::hram && index <= memory::hram_end) {
    return m_builtin.readHRAM(index - memory::hram);
  }

  else if (index == memory::IE) {
    return m_pintr.read(index);
  }

  else {
    std::cerr << "read from illegal address: " << index << '\n';
  }
}

void bus::writeBus(const std::size_t index, const byte val) noexcept {
  if (index >= memory::rom0 && index <= memory::romx_end) {
    m_cart.writeROM(index - memory::rom0, val);
  }

  else if (index >= memory::vram && index <= memory::vram_end) {
    m_PPU.writeVRAM(index - memory::vram, val);
  }

  else if (index >= memory::xram && index <= memory::xram_end) {
    m_cart.writeXRAM(index - memory::xram, val);
  }

  else if (index >= memory::wram0 && index <= memory::wramx_end) {
    m_builtin.writeWRAM(index - memory::wram0, val);
  }

  else if (index >= memory::echo && index <= memory::echo_end) {
    m_builtin.writeEcho(index - memory::echo, val);
  }

  else if (index >= memory::oam && index <= memory::oam_end) {
    m_PPU.writeOAM(index - memory::oam, val);
  }

  else if (index >= memory::noUsable && index <= memory::noUsable_end) {
    std::cerr << "write to no usable area\n";
    (void)val;
  }

  else if (index >= memory::io && index <= memory::io_end) {
    if (index == 0xFF0F) {
      m_pintr.write(index, val);
    } else if (index == 0xFF46) {
      m_PPU.writeDMA(val);
    } else if (index == 0xFF50 && (val & 0b0000'0001)) {
      m_cart.disableBootROM();
      (void)val;
    } else {
      m_regs.write(index - memory::io, val);
    }
  }

  else if (index >= memory::hram && index <= memory::hram_end) {
    m_builtin.writeHRAM(index - memory::hram, val);
  }

  else if (index == memory::IE) {
    m_pintr.write(index, val);
  }

  else {
    std::cerr << "write illegal to address: " << index << '\n';
  }
}

}
