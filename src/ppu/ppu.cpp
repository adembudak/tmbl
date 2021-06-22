#include "tmbl/config.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <array>
#include <cstddef>

namespace tmbl {

class registers;

ppu::ppu(registers &regs_, cartridge &cart_, interrupts &intr_)
    : m_regs(regs_), m_cart(cart_), m_intr(intr_), cgb_support(m_cart.cgbSupport()),
      STAT(m_regs.getAt(0xFF41), /*ly*/ m_regs.getAt(0xFF44), /*lyc*/ m_regs.getAt(0xFF45)),
      LCDC(m_regs.getAt(0xFF40), m_cart.cgbSupport()),      // lcd controller
      SCY(m_regs.getAt(0xFF42)), SCX(m_regs.getAt(0xFF43)), // screen (viewport) scroll
      LY(m_regs.getAt(0xFF44)), LYC(m_regs.getAt(0xFF45)),
      DMA(m_regs.getAt(0xFF46)),                              // direct memory access
      BGP(m_regs.getAt(0xFF47)),                              // background palette
      OBP0(m_regs.getAt(0xFF48)), OBP1(m_regs.getAt(0xFF49)), // object palettes
      WY(m_regs.getAt(0xFF4A)), WX(m_regs.getAt(0xFF4B)),     // window position
      VBK(m_regs.getAt(0xFF4F)),                              // color gameboy vram bank control
      HDMA1(m_regs.getAt(0xFF51)), HDMA2(m_regs.getAt(0xFF52)), HDMA3(m_regs.getAt(0xFF53)),
      HDMA4(m_regs.getAt(0xFF54)), HDMA5(m_regs.getAt(0xFF55)), // cgb DMAs
      BCPS(m_regs.getAt(0xFF68)), BCPD(m_regs.getAt(0xFF69)),   // cgb background palettes
      OCPS(m_regs.getAt(0xFF6A)), OCPD(m_regs.getAt(0xFF6B))    // cgb object palettes
{

  if (m_cart.cgbSupport()) {
    m_vram.resize(16_KB);
  } else {
    m_vram.resize(8_KB);
  }
}

void ppu::update(std::function<void(const tmbl::ppu::frame framebuffer)> drawCallback) {
  if (LCDC.lcdControllerStatus() == on) {

    switch (STAT.mode_flag()) {
      case stat::mode::SEARCHING_OAM: //////////////////////////////////////// mode 2
        m_intr.LCDC_Status_IRQ = STAT.matchSearchOAM();
        scanline();

        m_clock.wait(oamCycles);
        STAT.mode_flag(stat::mode::TRANSFERING_DATA_TO_LCD);
        break;

      case stat::mode::TRANSFERING_DATA_TO_LCD: ////////////////////////////// mode 3
        drawCallback(framebuffer);

        m_clock.wait(vramCycles);
        STAT.mode_flag(stat::mode::HORIZONTAL_BLANKING);
        break;

      case stat::mode::HORIZONTAL_BLANKING: ////////////////////////////////// mode 0
        ++LY;
        m_intr.LCDC_Status_IRQ = STAT.matchHblank();

        m_clock.wait(hblankCycles);
        // drawing scanline has finished,
        // now if not vertical blanking, scan other line

        if (STAT.match_flag()) { // LY == LYC
          m_intr.LCDC_Status_IRQ = STAT.matchCoincidence();
        }

        if (LY == screenHeight) {
          STAT.mode_flag(stat::mode::VERTICAL_BLANKING);
        } else {
          STAT.mode_flag(stat::mode::SEARCHING_OAM);
        }
        break;

      case stat::mode::VERTICAL_BLANKING: //////////////////////////////////// mode 1
        m_intr.VBlank_IRQ = STAT.matchVblank();

        for (uint8 i = 0; i < 10; ++i) { // 10 scanlines of vertical blank
          ++LY;
          m_clock.wait(scanlineCycles);
        }

        if (LY == screenVBlankHeight) {
          LY = 0;
        }

        STAT.mode_flag(stat::mode::SEARCHING_OAM);
        break;
    }
  } else {
    LY = 0;
    std::fill(framebuffer.at(LY).begin(), framebuffer.at(LY).end(), default_palette.at(4));
    STAT.mode_flag(stat::mode::VERTICAL_BLANKING);
  }
}

byte ppu::readVRAM(const std::size_t index) const noexcept {
  const std::size_t effective_index = 8_KB * vbk() + index; // 8KB is VRAM bank size
  return m_vram.at(effective_index);
}

void ppu::writeVRAM(const std::size_t index, const byte val) noexcept {
  const std::size_t effective_index = 8_KB * vbk() + index;
  m_vram.at(effective_index) = val;
}

byte ppu::readOAM(const std::size_t index) const noexcept { return m_oam.at(index); }

void ppu::writeOAM(const std::size_t index, const byte val) noexcept { m_oam.at(index) = val; }

void ppu::writeDMA(const byte val) {
  DMA = val;

  const std::size_t offset = 0x100 * val;
  std::copy_n(m_vram.begin() + offset, 160_B, m_oam.begin());

  m_clock.wait(160);
}

void ppu::fetchBackground() noexcept {
  // implement this
}

void ppu::fetchWindow() noexcept {
  // implement this
}

void ppu::fetchSprite() noexcept {
  // implement this
}

void ppu::scanline() noexcept {
  if (LCDC.bgDisplayStatus() == on) {
    fetchBackground();
  }

  if (LY >= WY && LCDC.winDisplayStatus() == on) {
    fetchWindow();
  }

  if (LCDC.objDisplayStatus() == on) {
    fetchSprite();
  }
}

uint8 ppu::vbk() const noexcept { return cgb_support ? (VBK & 0b0000'0001) : 0; }
}
