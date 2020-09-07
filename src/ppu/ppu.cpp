#include "tmbl/ppu/ppu.h"
#include <vector>

namespace tmbl {

ppu::ppu(std::shared_ptr<registers> pRegs, std::shared_ptr<cartridge> pCart,
         std::shared_ptr<interrupts> pIntr)
    : m_pRegs(pRegs), m_pCart(pCart), m_pIntr(pIntr), STAT(pRegs), LCDC(pRegs) {}

byte ppu::readVRAM(const std::size_t index) { return m_vram.at(index); }
void ppu::writeVRAM(const std::size_t index, const byte val) { m_vram.at(index) = val; }

byte ppu::readOAM(const std::size_t index) { return m_oam.at(index); }
void ppu::writeOAM(const std::size_t index, const byte val) { m_oam.at(index) = val; }

}
