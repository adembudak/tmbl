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

  cflag lcdControllerStatus() const noexcept;             // bit 7
  std::pair<uint16, uint16> chrCodeArea() const noexcept; // bit 6 map select
  cflag windowStatus() const noexcept;                    // bit 5
  std::pair<uint16, uint16> bgChrArea() const noexcept;   // bit 4 tile block select
  std::pair<uint16, uint16> bgCodeArea() const noexcept;  // bit 3 map select
  std::pair<uint8, uint8> objSize() const noexcept;       // bit 2
  cflag objDisplayStatus() const noexcept;                // bit 1
  cflag bgDisplayStatus() const noexcept;                 // bit 0

private:
  byte &m_value;
  bool cgb_support = false;
};

}

#endif

