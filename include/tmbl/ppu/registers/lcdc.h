#ifndef LCDC_H
#define LCDC_H

#include "../../io/registers.h"

#include <memory>

namespace tmbl {

class lcdc {
public:
  lcdc(std::shared_ptr<registers> pRegs);

  // https://archive.org/details/GameBoyProgManVer1.1/page/n56/mode/1up

  cflag bgDisplayStatus() const noexcept;
  cflag objDisplayStatus() const noexcept;
  std::pair<n8, n8> objSize() const noexcept;
  std::pair<n16, n16> bgCodeArea() const noexcept;
  std::pair<n16, n16> bgChrArea() const noexcept;
  cflag windowStatus() const noexcept;
  std::pair<n16, n16> windowCodeArea() const noexcept;
  cflag lcdControllerStatus() const noexcept;

private:
  std::shared_ptr<registers> m_pRegs;
  byte &LCDC = m_pRegs->getAt(0xFF40);
};

}

#endif
