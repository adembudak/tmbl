#ifndef OBJ_H
#define OBJ_H

#include "tmbl/config.h"

// https://gbdev.io/pandocs/OAM.html#vram-sprite-attribute-table-oam
//
//
// https://archive.org/details/GameBoyProgManVer1.1/page/n62/mode/1up

namespace tmbl {

class obj {
public:
  obj(const byte y, const byte x, const byte tile_num, const byte attrib);

  uint8 x() const noexcept;
  uint8 y() const noexcept;

  bool priority() const noexcept;
  bool yFlip() const noexcept;
  bool xFlip() const noexcept;
  uint8 dmgPalette() const noexcept;

  uint8 vramBank() const noexcept;
  uint8 cgbPalette() const noexcept;

private:
  byte y_pos;
  byte x_pos;

  byte tile_number;
  byte attribute;
};

}

#endif
