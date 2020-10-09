#include "tmbl/config.h"
#include "tmbl/ppu/ppu.h"

#include <array>
#include <cstddef>
#include <memory>

namespace tmbl {

class cartridge;
class interrupts;
class registers;

ppu::ppu(std::shared_ptr<registers> pRegs, std::shared_ptr<cartridge> pCart,
         std::shared_ptr<interrupts> pIntr)
    : m_pRegs(pRegs), m_pCart(pCart), m_pIntr(pIntr), STAT(pRegs), LCDC(pRegs),
      SCY(m_pRegs->getAt(0xFF42)), SCX(m_pRegs->getAt(0xFF43)), LY(m_pRegs->getAt(0xFF44)),
      LYC(m_pRegs->getAt(0xFF45)), DMA(m_pRegs->getAt(0xFF46)), BGP(m_pRegs->getAt(0xFF47)),
      OBP0(m_pRegs->getAt(0xFF48)), OBP1(m_pRegs->getAt(0xFF49)), WY(m_pRegs->getAt(0xFF4A)),
      WX(m_pRegs->getAt(0xFF4B)), VBK(m_pRegs->getAt(0xFF4F)), HDMA1(m_pRegs->getAt(0xFF51)),
      HDMA2(m_pRegs->getAt(0xFF52)), HDMA3(m_pRegs->getAt(0xFF53)), HDMA4(m_pRegs->getAt(0xFF54)),
      HDMA5(m_pRegs->getAt(0xFF55)), BCPS(m_pRegs->getAt(0xFF68)), BCPD(m_pRegs->getAt(0xFF69)),
      OCPS(m_pRegs->getAt(0xFF6A)), OCPD(m_pRegs->getAt(0xFF6B))

{}

byte ppu::readVRAM(const std::size_t index) { return m_vram.at(index); }
void ppu::writeVRAM(const std::size_t index, const byte val) { m_vram.at(index) = val; }

byte ppu::readOAM(const std::size_t index) { return m_oam.at(index); }
void ppu::writeOAM(const std::size_t index, const byte val) { m_oam.at(index) = val; }

}
