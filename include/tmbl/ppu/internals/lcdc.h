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
  // Code block                 | tile set
  //

  // clang-format off
  
  // VRAM structure
  //   |                  (each tile is 16 bytes)                  | (1KB = 32x32 = [0,1024) indexes)
  //   |  2KB = 128 tiles  |  2KB = 128 tiles  |  2KB = 128 tiles  | 1KB      | 1KB      | = 8KB total
  //   |                   |                   |                   |          |          |
  //   |   Block 0         |    Block 1        |    Block 2        |*LCDC.3=0*|*LCDC.3=1*|
  //   |              W I N D O W              |                   |          |          |
  //   |+++++++++++++++LCDC.4=1++++++++++++++++|                   |          |          |
  //   |++tile [0,128)+++++++++tile [128,256)++|                   |,LCDC.6=0,|,LCDC.6=1,|
  //   |                   |          B A C K G R O U N D          |          |          |
  //   |                   |~~~~~~~~~~~~~~~~LCDC.4=0~~~~~~~~~~~~~~~|          |          |
  //   |                   |~~tile [-128,0)~~~~~~tile [0,128)~~~~~~|          |          |
  //   [------------------)[------------------)[------------------)[---------)[----------)
  //   0x8000              0x8800              0x9000              0x9800     0x9C00     0xA000
  //
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  //                                                                                   1 | 0  
  //--------------------------------------------------------------------------------------------------
  cflag                     lcdControllerStatus() const noexcept; // bit 7            on | off 
  std::pair<uint16, uint16>    winTileMapSelect() const noexcept; // bit 6  [9C00, 9FFF) | [9800, 9C00) 
  cflag                        winDisplayStatus() const noexcept; // bit 5            on | off 
  std::pair<uint16, bool>         bgBlockSelect() const noexcept; // bit 4  [8000, 9000) | [8800, 9800) 
  std::pair<uint16, bool>        winBlockSelect() const noexcept; // bit 4  [8000, 9000) | [8800, 9800) 
  std::pair<uint16, uint16>     bgTileMapSelect() const noexcept; // bit 3  [9C00, A000) | [9800, 9C00) 
  std::pair<uint8, uint8>               objSize() const noexcept; // bit 2        8 x 16 | 8 x 8 
  cflag                        objDisplayStatus() const noexcept; // bit 1            on | off 
  cflag                         bgDisplayStatus() const noexcept; // bit 0            on | off 

private:
  byte &m_value;
  bool cgb_support = false;
};

}

#endif
