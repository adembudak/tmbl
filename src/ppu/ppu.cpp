#include "tmbl/config.h"
#include "tmbl/ppu/ppu.h"

#include <array>
#include <cstddef>
#include <memory>

namespace tmbl {

class cartridge;
class interrupts;
class registers;

// clang-format off
ppu::ppu(registers &regs_, cartridge &cart_, interrupts &intr_)
    : m_regs(regs_), m_cart(cart_), m_intr(intr_), 
	STAT(m_regs.getAt(0xFF41)), LCDC(m_regs.getAt(0xFF40)),

        SCY(m_regs.getAt(0xFF42)), SCX(m_regs.getAt(0xFF43)), 

	LY(m_regs.getAt(0xFF44)), LYC(m_regs.getAt(0xFF45)), 

	DMA(m_regs.getAt(0xFF46)), 

	BGP(m_regs.getAt(0xFF47)), OBP0(m_regs.getAt(0xFF48)), OBP1(m_regs.getAt(0xFF49)), 

	WY(m_regs.getAt(0xFF4A)), WX(m_regs.getAt(0xFF4B)), 

	VBK(m_regs.getAt(0xFF4F)), 

	HDMA1(m_regs.getAt(0xFF51)), HDMA2(m_regs.getAt(0xFF52)), HDMA3(m_regs.getAt(0xFF53)), 
	HDMA4(m_regs.getAt(0xFF54)), HDMA5(m_regs.getAt(0xFF55)), 

	BCPS(m_regs.getAt(0xFF68)), BCPD(m_regs.getAt(0xFF69)), 
	OCPS(m_regs.getAt(0xFF6A)), OCPD(m_regs.getAt(0xFF6B))

{}

byte ppu::readVRAM(const std::size_t index) { return m_vram.at(index); }
void ppu::writeVRAM(const std::size_t index, const byte val) { m_vram.at(index) = val; }

byte ppu::readOAM(const std::size_t index) { return m_oam.at(index); }
void ppu::writeOAM(const std::size_t index, const byte val) { m_oam.at(index) = val; }

}
