#include "memory.h"
#include "../cpu/registers/reg16.h"
#include "../cpu/registers/reg8.h"
#include "../config.h"
#include <algorithm>

namespace tmbl::memory {

memory::memory() {
  std::copy(std::begin(nintendo_logo), std::end(nintendo_logo), std::data(m_data) + 0x104);
}

[[nodiscard]] bool memory::dumpROM(const char *romfile) { return {}; }
[[nodiscard]] byte *memory::data() const noexcept { return m_data.data(); }

[[nodiscard]] byte &memory::operator[](const u16 nn) { return m_data.at(nn); }

byte &memory::IP() noexcept { return m_data[0xFF00]; }
byte &memory::SB() noexcept { return m_data[0xFF01]; }
byte &memory::SC() noexcept { return m_data[0xFF02]; }
byte &memory::DIV() noexcept { return m_data[0xFF04]; }
byte &memory::TIMA() noexcept { return m_data[0xFF05]; }
byte &memory::TMA() noexcept { return m_data[0xFF06]; }
byte &memory::TAC() noexcept { return m_data[0xFF07]; }

// bank kontrol registers -CGB only
byte &memory::KEY1() noexcept { return m_data[0xFF4D]; }
byte &memory::RP() noexcept { return m_data[0xFF56]; }

// Interrupt flags
byte &memory::IF() noexcept { return m_data[0xFF0F]; }
byte &memory::IE() noexcept { return m_data[0xFFFF]; }

// display registers
byte &memory::LCDC() noexcept { return m_data[0xFF40]; }
byte &memory::STAT() noexcept { return m_data[0xFF41]; }
byte &memory::SCY() noexcept { return m_data[0xFF42]; }
byte &memory::SCX() noexcept { return m_data[0xFF43]; }
byte &memory::LY() noexcept { return m_data[0xFF44]; }
byte &memory::LYC() noexcept { return m_data[0xFF45]; }
byte &memory::DMA() noexcept { return m_data[0xFF46]; }
byte &memory::BGP() noexcept { return m_data[0xFF47]; }
byte &memory::OBP0() noexcept { return m_data[0xFF48]; }
byte &memory::OBP1() noexcept { return m_data[0xFF49]; }
byte &memory::WY() noexcept { return m_data[0xFF4A]; }
byte &memory::WX() noexcept { return m_data[0xFF4B]; }

// display registers -  CGB only
byte &memory::HDMA1() noexcept { return m_data[0xFF51]; }
byte &memory::HDMA2() noexcept { return m_data[0xFF52]; }
byte &memory::HDMA3() noexcept { return m_data[0xFF53]; }
byte &memory::HDMA4() noexcept { return m_data[0xFF54]; }
byte &memory::HDMA5() noexcept { return m_data[0xFF55]; }
byte &memory::BCPS() noexcept { return m_data[0xFF68]; }
byte &memory::BCPD() noexcept { return m_data[0xFF69]; }
byte &memory::OCPS() noexcept { return m_data[0xFF6A]; }
byte &memory::OCPD() noexcept { return m_data[0xFF6B]; }


}
