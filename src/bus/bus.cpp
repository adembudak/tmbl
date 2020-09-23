#include "tmbl/bus/bus.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/config.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/io/registers.h"
#include "tmbl/memory_map.h"
#include "tmbl/ppu/ppu.h"

namespace tmbl {

bus::bus(std::shared_ptr<cartridge> pCart, std::shared_ptr<registers> pRegs,
         std::shared_ptr<interrupts> pIntr, std::shared_ptr<builtin> pBuiltin,
         std::shared_ptr<ppu> pPPU)
    : m_pCart(pCart), m_pRegs(pRegs), m_pIntr(pIntr), m_pBuiltin(pBuiltin), m_pPPU(pPPU) {}

byte bus::readBus(const std::size_t index) {
  if (index >= memory::rom && index <= memory::rom_end) {
    m_pCart->read(index);
  } else if (index >= memory::vram && index <= memory::vram_end) {
    return m_pPPU->readVRAM(index);
  } else if (index >= memory::xram && index <= memory::xram_end) {
    return m_pCart->read(index);
  } else if (index >= memory::wram && index <= memory::wram_end) {
    return m_pBuiltin->readWRAM(index);
  } else if (index >= memory::echo && index <= memory::echo_end) {
    /// return buildin->read(index) ;
  } else if (index >= memory::oam && index <= memory::oam_end) {
    return m_pPPU->readOAM(index);
  } else if (index >= memory::io && index <= memory::io_end) {
    if (index == 0xFF0F)
      return m_pIntr->IF;
    return m_pRegs->read(index);
  } else if (index >= memory::hram && index <= memory::hram_end) {
    return m_pBuiltin->readHRAM(index);
  } else {
    return m_pIntr->IE;
  }
}

}
