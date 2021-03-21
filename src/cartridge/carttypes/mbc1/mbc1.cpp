#include "tmbl/config.h"
#include "tmbl/cartridge/carttypes/mbc1/mbc1.h"

#include <cstddef>
#include <vector>
#include <algorithm>

/*
https://retrocomputing.stackexchange.com/questions/11732/how-does-the-gameboys-memory-bank-switching-work

0x0000 - 0x3FFF: ROM Bank $00 (Read Only)
0x4000 - 0x7FFF: Additional ROM Banks (Read Only)
0xA000 - 0xBFFF: RAM Bank, if present (Read/Write)
0x0000 - 0x1FFF: RAM Enable (Write Only)
0x2000 - 0x3FFF: ROM Bank Number (Write Only)
0x6000 - 0x7FFF: ROM/RAM Mode Select (Write Only)
0x6000 - 0x7FFF: ROM/RAM Mode Select (Write Only)
0x4000 - 0x5FFF: RAM Bank Number or Upper Bits of ROM Bank Number (Write Only)
*/

namespace tmbl {

/*
$0000-$3FFF: ROM Bank $00 (Read Only)

This always contains the first 16KB of the cartridge, the first memory bank. It is unable to be
switched or modified.

$4000-$7FFF: Additional ROM Banks (Read Only)

This area of memory will contain whatever memory bank is currently selected.

$A000-$BFFF: Ram Bank, if present (Read/Write)

This is where RAM banks will be placed in memory. They are able to be both written to and read from,
allowing the game to store data. If the cartridge contains a battery, like in Pokemon cartridges,
the data written is able to be preserved across shutdowns. This type of MBC had 3 different RAM
options: - 2KB: $A000-A7FF - 8KB: $A000-BFFF - 32KB: $A000-BFFF (four switchable 8KB banks)

$0000-$1FFF: RAM Enable (Write Only)

Before a game is able to use the RAM, it must be specifically enabled. This is done by writing a
value with the lower 4 bits being $0A somewhere in this address space. To disable RAM, any number
except $0A can be written. It does not matter where it is written, just as long as it within the
address range. You will notice that this address range is part of the first ROM bank, which is read
only. Because it is ROM, there is obviously no way to actually write data to those memory locations.
Instead, the write call is "intercepted" and interpreted differently by the MBC. This method of
writing to ROM is also used for the remaining memory areas I'll explain now.

$2000-$3FFF: ROM Bank Number (Write Only)

Writing a value to this address range will select the lower 5 bits of the bank number. There are a
few special cases though. If the value $00 is written, it will converted to bank $01. This is not an
issue because bank $00 is always present at $0000-$3FFF. The issue lies in writing the values $20,
$40, and $60. When these values are written, instead of addressing the correct ROM banks they will
address banks $21``$41 and $61 respectively. I couldn't find an explanation of why this takes place,
but I assume it has something to do with how the lower 5 bits are used when choosing the bank. Each
of these numbers have all zeros as the lower 5 bits (0x0XX00000). This issue is not present in MBC2
and MBC3.

$6000-$7FFF: ROM/RAM Mode Select (Write Only)

Writing either $00 or $01 to this area will select which mode the MBC is in. On an MBC1, there are
two modes: 16Mb ROM/8KB RAM and 4Mb ROM/32KB RAM. The game is able to switch between the modes on
the fly, allowing a game to access extended ROM banks during normal operation and switch to RAM mode
temporarily when data needs to be read. Valid values are $00 for ROM mode and $01 for RAM mode.

$4000-$5FFF: RAM Bank Number or Upper Bits of ROM Bank Number (Write Only)

Writing to this area of memory will effect the ROM or RAM modes, depending on what is written in
$6000-$7FFF. Only the first two bits of the value matter. If in ROM mode (no RAM bank switching), it
will specify the upper two bits of the ROM bank number. In this mode, only RAM bank $00 may be used.

If in RAM mode, it will specify which RAM bank to load into $A000-$BFFF. In this mode, only ROM
banks $00-$1f may be used.

The MBC1 is only able to switch between 125 different ROM banks, instead of the expected 128. The
ROM banks are addressed using a 6 bit number created from writing to $2000-$3FFF and $4000-$5FFF.
The reason is because of the number conversion when writing $20, $40, and $60 to $2000-$3FFF. This
results in it being able to address 128-3 or 125 banks for a actual total of 1.95MB, not 2MB. This
issue is not present in MBC2 and MBC3. MBC2 has a maximum of 16 banks, so the issue is never
encountered. The MBC3 chip correctly addresses $20, $40, and $60 and does not perform the
conversion.
*/

mbc1::mbc1(std::vector<byte> &&rom, std::size_t xram_size) : m_rom(std::move(rom)) {
  if (xram_size != 0) {
    ram_available = true;
    m_xram.resize(xram_size);
    if (xram_size > 8_KB) {
      ram_banking_available = true;
    }
  }

  if (auto rom_size = m_rom.size(); rom_size > 32_KB) {
    rom_banking_available = true;
    if (rom_size == 256_KB) { // adjust lower rom mask, and upper rom mask shift (5 by default)
      lower_rom_mask >>= 1;
      lower_rom_mask_countl_one -= 1;
    } else if (rom_size == 128_KB) {
      lower_rom_mask >>= 2;
      lower_rom_mask_countl_one -= 2;
    } else if (rom_size == 64_KB) {
      lower_rom_mask >>= 3;
      lower_rom_mask_countl_one -= 3;
    } else {
    }
  }
}

void mbc1::write(const std::size_t index, const byte val) {

  if (index >= 0x0000 && index <= 0x1FFF) { // register 1, ram chip select
    ram_access_enabled = ((val & 0b0000'1111) == 0x0A);
  }

  else if (index >= 0x2000 && index <= 0x3FFF) { // register 2, lower rom bank select
    if ((val & 0x1F) == 0) {
      lower_rom_bank = val + 1;
      return;
    }

    lower_rom_bank = val & lower_rom_mask;
  }

  else if (index >= 0x4000 && index <= 0x5FFF) { // register 3, decide upper rom/ram bank number
    if (banking_mode == mode::rom_banking) {     // rom banking mode
      upper_rom_mask = (m_rom.size() >= 512_KB) ? val & upper_rom_mask : 0;
      ram_bank = 0;
    } else { // ram banking mode
      ram_bank = ram_banking_available ? val & ram_mask : 0;
    }
  }

  else if (index >= 0x6000 && index <= 0x7FFF) {          // register 4, decide banking mode
    if (m_rom.size() > 512_KB || ram_banking_available) { // mode0: rom banking, mode1: ram banking
      banking_mode = (val & 0b0000'0001) ? mode::ram_banking : mode::rom_banking;
    }
  }

  else if (index >= 0xA000 && index <= 0xBFFF) {
    if (ram_available && ram_access_enabled) {
      m_xram.at((ram_bank * ram_bank_width) + index) = val;
    }
  }
}

byte mbc1::read(const std::size_t index) {
  if (index >= 0x0000 && index <= 0x3FFF) {
    effective_rom_bank =
        (banking_mode == mode::rom_banking) ? 0 : (upper_rom_bank << lower_rom_mask_countl_one);

    return m_rom.at((effective_rom_bank * rom_bank_width) + index);
  }

  else if (index >= 0x4000 && index <= 0x7FFF) {
    effective_rom_bank = (upper_rom_bank << lower_rom_mask_countl_one) | lower_rom_bank;

    return m_rom.at((effective_rom_bank * rom_bank_width) + index);
  }

  else if (index >= 0xA000 && index <= 0xBFFF) {

    return (ram_available && ram_access_enabled) ? m_xram.at((ram_bank * ram_bank_width) + index)
                                                 : 0xFF;
  }
}

}

