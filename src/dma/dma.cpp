#include <tmbl/dma/dma.h>
#include <tmbl/ppu/ppu.h>
#include <tmbl/io/registers.h>
#include <tmbl/builtin/builtin.h>
#include <tmbl/cartridge/cartridge.h>
#include <tmbl/memory_map.h>

#include <cassert>

namespace tmbl {

dma::dma(cartridge &cart_, ppu &ppu_, registers &regs_, builtin &builtin_)
    : m_ppu(ppu_), m_regs(regs_), m_builtin(builtin_), m_cart(cart_), DMA(regs_.getAt(0xFF46)),
      HDMA1(regs_.getAt(0xFF51)), HDMA2(regs_.getAt(0xFF52)), HDMA3(regs_.getAt(0xFF53)),
      HDMA4(regs_.getAt(0xFF54)), HDMA5(regs_.getAt(0xFF55)) {}

void dma::dmaAction(const byte val) noexcept {
  DMA = val;
  const std::size_t address = 0x100 * DMA;

  if (m_cart.cgbSupport()) {                                      //// on CGB
    if (address >= memory::rom0 && address <= memory::romx_end) { // ROM -> OAM
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

  else { //// on DMG
    for (int i = 0; i < 160; ++i) {
      m_ppu.writeOAM(i, m_ppu.readVRAM((address - memory::vram) + i)); // VRAM -> OAM
    }
  }

  //  m_clock.wait(160);
}

void dma::hdmaAction(const byte val) noexcept { // ROM, XRAM, WRAM -> VRAM
  HDMA5 = val;

  const std::size_t transfer_source_address = hdmaSource();
  const std::size_t transfer_destination_address = hdmaDestination();
  const uint16 number_of_bytes_to_tranfer = hdmaAmountOfBytesToTranfer();

  if (HDMA5 & 0b1000'0000) { // HBlank DMA
    if (m_ppu.status() != statMode::HORIZONTAL_BLANKING) {
      // implement this
    }
  }

  else { // General DMA
    if (m_ppu.status() == statMode::VERTICAL_BLANKING) {
      // implement this
    }
  }
}

uint16 dma::hdmaSource() const noexcept {
  const uint16 source = (HDMA1 << 8U) | (HDMA2 & 0b1111'0000);
  return source;
}

uint16 dma::hdmaDestination() const noexcept {
  const uint16 destination = ((HDMA3 & 0b0001'1111) << 8U) | (HDMA4 & 0b1111'0000);
  return destination;
}

uint16 dma::hdmaAmountOfBytesToTranfer() const noexcept {
  const uint8 blocks = HDMA5 & 0b0111'1111;
  return (blocks + 1) * 16; // 16 is size of a tile
}

}
