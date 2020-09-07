#include "tmbl/cartridge/rom/rom.h"

namespace tmbl {

rom::rom(std::vector<byte> &rom_) : m_rom(std::move(rom_)) {}

byte rom::read_rom(const std::size_t index) { return m_rom.at(index); }
byte rom::read_xram(const std::size_t index) { return m_xram.at(index); }

}
