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

  // clang-format off
  cflag                    lcdControllerStatus() const noexcept; // bit 7, lcd on/off?
  std::pair<uint16, uint16>   chrMapAreaSelect() const noexcept; // bit 6, screen area select
  cflag                           windowStatus() const noexcept; // bit 5, window on/off?
  std::pair<uint16, bool>     bgChrBlockSelect() const noexcept; // bit 4, tile block select
  std::pair<uint16, uint16>    bgMapAreaSelect() const noexcept; // bit 3, screen area select
  std::pair<uint8, uint8>              objSize() const noexcept; // bit 2, big or small sprite?
  cflag                       objDisplayStatus() const noexcept; // bit 1, sprite on/off?
  cflag                        bgDisplayStatus() const noexcept; // bit 0, bg on/off?

private:
  byte &m_value;
  bool cgb_support = false;
};

}

#endif
