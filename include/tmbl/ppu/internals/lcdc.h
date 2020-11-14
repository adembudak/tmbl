#ifndef LCDC_H
#define LCDC_H

#include "tmbl/config.h"
#include "../../io/registers.h"

namespace tmbl {

class lcdc {
public:
  explicit lcdc(byte &val_, bool cgb_support);

  // https://archive.org/details/GameBoyProgManVer1.1/page/n56/mode/1up
  // terminology:
  // Gameboy Programming Manual | Gameboy PanDocs
  // -------------------------------------
  // Object(OBJ)                | Sprite
  // Character(CHR)             | Window
  // Background(BG)             | <-
  // Code area                  | tile map

  cflag lcdControllerStatus() const noexcept;                // bit 7
  std::pair<uint16, uint16> windowCodeArea() const noexcept; // map select
  cflag windowStatus() const noexcept;
  std::pair<uint16, uint16> bgChrArea() const noexcept;
  std::pair<uint16, uint16> bgCodeArea() const noexcept; // map select
  std::pair<uint8, uint8> objSize() const noexcept;
  cflag objDisplayStatus() const noexcept;
  cflag bgDisplayStatus() const noexcept; // bit 0

private:
  byte &m_value;
  bool cgb_support = false;
};

}

#endif

