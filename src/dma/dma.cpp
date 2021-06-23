#include <tmbl/dma/dma.h>
#include <tmbl/ppu/ppu.h>
#include <tmbl/io/registers.h>
#include <tmbl/builtin/builtin.h>
#include <tmbl/cartridge/cartridge.h>
#include <tmbl/memory_map.h>

#include <cassert>

namespace tmbl {

dma::dma(ppu &ppu_, registers &regs_, builtin &builtin_, cartridge &cart_)
    : m_ppu(ppu_), m_regs(regs_), m_builtin(builtin_), m_cart(cart_), DMA(m_regs.getAt(0xFF46)),
      HDMA1(m_regs.getAt(0xFF51)), HDMA2(m_regs.getAt(0xFF52)), HDMA3(m_regs.getAt(0xFF53)),
      HDMA4(m_regs.getAt(0xFF54)), HDMA5(m_regs.getAt(0xFF55)) {}

void dma::dmaAction() noexcept {

  if (const std::size_t address = 0x100 * DMA; m_cart.cgbSupport()) { // CGB
    if (address >= memory::rom0 && address <= memory::romx_end) {     // ROM -> OAM
      // reading rom in cgb involves banking so raw loop is required
      for (int i = 0; i < 160; ++i) {
        m_ppu.writeOAM(i, m_cart.readROM(address + i));
      }
    }

    else if (address >= memory::vram && address <= memory::vram_end) { // VRAM -> OAM
      for (int i = 0; i < 160; ++i) {
        m_ppu.writeOAM(i, m_ppu.readVRAM((address - memory::vram) + i));
      }
    }

    else if (address >= memory::xram && address <= memory::xram_end) { // XRAM -> OAM
      for (int i = 0; i < 160; ++i) {
        m_ppu.writeOAM(i, m_cart.readXRAM((address - memory::xram) + i));
      }
    }

    else if (address >= memory::wram0 && address <= memory::wramx_end) { // WRAM -> OAM
      for (int i = 0; i < 160; ++i) {
        m_ppu.writeOAM(i, m_builtin.readWRAM((address - memory::wram0) + i));
      }
    }

    else {
      assert(false);
    }
  }

  else { // DMG
    for (int i = 0; i < 160; ++i) {
      m_ppu.writeOAM(i, m_ppu.readVRAM((address - memory::vram) + i)); // VRAM -> OAM
    }
  }

  //  m_clock.wait(160);
}

void dma::hdmaAction() noexcept {
  // implement this
}

uint16 dma::hdmaSource() const noexcept {
  const uint16 source = (HDMA1 << 8U) | (HDMA2 & 0b1111'0000);
  return source;
}

uint16 dma::hdmaDestination() const noexcept {
  const uint16 destination = ((HDMA3 & 0b0001'1111) << 8U) | (HDMA4 & 0b1111'0000);
  return destination;
}
}
