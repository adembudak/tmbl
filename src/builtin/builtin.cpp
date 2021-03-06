#include "tmbl/config.h"
#include "tmbl/memory_map.h"
#include "tmbl/utils/utils.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/io/registers.h"
#include "tmbl/cartridge/cartridge.h"

#include <algorithm>

namespace tmbl {
builtin::builtin(cartridge &cart_, registers &regs_)
    : m_regs(regs_), SVBK(regs_.getAt(0xFF70)), m_cart(cart_),
      color_gameboy_support(cart_.cgbSupport()) {}

byte builtin::readWRAM(const std::size_t index) const noexcept {
  if (color_gameboy_support) {
    uint8 wram_bank = SVBK & 0b0000'0111;
    if (wram_bank == 0) {
      wram_bank = 1;
    }

    // 4KB is wram bank size
    const std::size_t effective_index = (wram_bank * 4_KB) + index;

    return m_wram[effective_index];
  } else {
    return m_wram[index];
  }
}

void builtin::writeWRAM(const std::size_t index, const byte val) noexcept {
  if (color_gameboy_support) {
    uint8 wram_bank = SVBK & 0b0000'0111;
    if (wram_bank == 0) {
      wram_bank = 1;
    }

    const std::size_t effective_index = (wram_bank * 4_KB) + index;

    m_wram[effective_index] = val;
  } else {
    m_wram[index] = val;
  }
}

byte builtin::readHRAM(const std::size_t index) const noexcept { return m_hram[index]; }

void builtin::writeHRAM(const std::size_t index, const byte val) noexcept { m_hram[index] = val; }

byte builtin::readEcho(const std::size_t index) const noexcept { return readWRAM(index + 512_B); }

void builtin::writeEcho(const std::size_t index, const byte val) noexcept {
  writeWRAM(index + 512_B, val);
}

void builtin::adjustWRAMSize() noexcept {
  if (color_gameboy_support) {
    m_wram.resize(32_KB);
  } else {
    m_wram.resize(8_KB);
  }

  std::generate(m_wram.begin(), m_wram.end(), []() { return utils::randomByte(); });
}

}
