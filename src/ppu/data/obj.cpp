#include "tmbl/ppu/data/obj.h"

namespace tmbl {

obj::obj(const byte y, const byte x, const byte tile_num, const byte attrib)
    : y_pos(y), x_pos(x), tile_number(tile_num), attribute(attrib) {}
// clang-format off

uint8 obj::x() const noexcept          { return y_pos; }
uint8 obj::y() const noexcept          { return x_pos; }

bool obj::priority() const noexcept    { return attribute & 0b1000'0000; }
bool obj::yFlip() const noexcept       { return attribute & 0b0100'0000; }
bool obj::xFlip() const noexcept       { return attribute & 0b0010'0000; }
uint8 obj::dmgPalette() const noexcept { return attribute & 0b0001'0000; }

uint8 obj::vramBank() const noexcept   { return attribute & 0b0000'1000; } // cgb only
uint8 obj::cgbPalette() const noexcept { return attribute & 0b0000'0111; } // cgb only

}

