#include "tmbl/ppu/data/obj.h"

namespace tmbl {

obj::obj(const byte y, const byte x, const byte tile_num, const byte attrib)
    : y_pos(y), x_pos(x), tile_number(tile_num) {
  // clang-format off
  obj_bg_priority =    attrib & 0b1000'0000;
  y_flip =             attrib & 0b0100'0000;
  x_flip =             attrib & 0b0010'0000;
  palette_number =     attrib & 0b0001'0000; // 0:obp0 1:obp1
  vram_bank =          attrib & 0b0000'1000; // 0:bank0 1:bank1:
  cgb_palette_number = attrib & 0b0000'0111; // obp0-7
}

}

