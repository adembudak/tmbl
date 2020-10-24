#include "tmbl/config.h"
#include "tmbl/cartridge/rom/rom.h"
#include <utility>

namespace tmbl {

rom::rom(std::vector<char> &rom_) : m_rom(std::move(rom_)) {}

byte rom::read_rom(const std::size_t index) { return m_rom.at(index); }
byte rom::read_xram(const std::size_t index) { return m_xram.at(index); }
byte rom::write_xram(const std::size_t index, const byte val) { return m_xram.at(index) = val; }

}
