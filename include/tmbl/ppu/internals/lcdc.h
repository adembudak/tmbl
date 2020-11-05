#ifndef LCDC_H
#define LCDC_H

#include "tmbl/config.h"
#include "../../io/registers.h"

namespace tmbl {

class lcdc {
public:
  explicit lcdc(byte &val_);

  // https://archive.org/details/GameBoyProgManVer1.1/page/n56/mode/1up

  cflag bgDisplayStatus() const noexcept;
  cflag objDisplayStatus() const noexcept;
  std::pair<uint8, uint8> objSize() const noexcept;
  std::pair<uint16, uint16> bgCodeArea() const noexcept;
  std::pair<uint16, uint16> bgChrArea() const noexcept;
  cflag windowStatus() const noexcept;
  std::pair<uint16, uint16> windowCodeArea() const noexcept;
  cflag lcdControllerStatus() const noexcept;

private:
  byte &m_value;
};

}

#endif

