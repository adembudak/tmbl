#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include <cstddef>
// clang-format off

namespace tmbl::memory {

// GameBoy memory layout
//                                              address |  size  |   located in
// -----------------------------------------------------------------------------
static constexpr std::size_t rom0 =             0x0000; // 32KB      Cartridge
static constexpr std::size_t rom0_end =         0x3FFF;
static constexpr std::size_t romx =             0x4000;
static constexpr std::size_t romx_end =         0x7FFF; 

static constexpr std::size_t vram =             0x8000; // 8KB       PPU
static constexpr std::size_t vram_end =         0x9FFF; 

static constexpr std::size_t xram =             0xA000; // 8KB       Cartridge
static constexpr std::size_t xram_end =         0xBFFF;  

static constexpr std::size_t wram0 =            0xC000; // 8KB       Built-in
static constexpr std::size_t wram0_end =        0xCFFF;
static constexpr std::size_t wramx =            0xD000;
static constexpr std::size_t wramx_end =        0xDFFF;

static constexpr std::size_t echo =             0xE000; // 7KB+512B  Built-in
static constexpr std::size_t echo_end =         0xFDFF;

static constexpr std::size_t oam =              0xFE00; // 160B      PPU
static constexpr std::size_t oam_end =          0xFE9F;

static constexpr std::size_t noUsable =         0xFEA0; // 96B       Built-in
static constexpr std::size_t noUsable_end =     0xFEFF;

static constexpr std::size_t io =               0xFF00; // 128B      IO
static constexpr std::size_t io_end =           0xFF7F;

static constexpr std::size_t hram =             0xFF80; // 127B      Built-in
static constexpr std::size_t hram_end =         0xFFFE;

static constexpr std::size_t IE =               0xFFFF; // 1B        Intterupts

                                               // Total:  32KB
}

#endif

