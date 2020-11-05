#ifndef OBP_H
#define OBP_H

#include "tmbl/config.h"
#include "../../io/registers.h"

#include <array>

namespace tmbl {

class obp {
public:
  explicit obp(byte &val_);
  int objPalette(const uint8 val);
  std::array<int, 4> objPalette();

private:
  byte &m_value;
};

}

#endif