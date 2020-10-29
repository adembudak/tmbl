#ifndef BGP_H
#define BGP_H

#include "tmbl/config.h"
#include "../../io/registers.h"

#include <memory>
#include <array>

namespace tmbl {

class bgp {
public:
  explicit bgp(byte &val_);
  int bgPalette(const uint8 val);
  std::array<int, 4> bgPalette();

private:
  std::shared_ptr<registers> m_pRegs;
  byte &m_value;
};

}

#endif

