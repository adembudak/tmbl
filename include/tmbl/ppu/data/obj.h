#ifndef OBJ_H
#define OBJ_H

#include "tmbl/config.h"

// https://gbdev.io/pandocs/#vram-sprite-attribute-table-oam
// https://archive.org/details/GameBoyProgManVer1.1/page/n62/mode/1up

namespace tmbl {

class obj {
public:
  obj(const byte y, const byte x, const byte tile_num, const byte attrib);

private:
  byte y_pos;
  byte x_pos;

  byte tile_number;

  flag obj_bg_priority;
  flag y_flip;
  flag x_flip;
  flag palette_number;
  flag vram_bank;          // cgb only
  byte cgb_palette_number; // cgb only
};

}

#endif

