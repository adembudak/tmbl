#ifndef N8_H
#define N8_H
#include "../../config.h"

namespace tmbl {

class n8 final {
public:
  n8(const uint8 n);

  uint8 loNibble() const noexcept;
  uint8 hiNibble() const noexcept;

  uint8 value() const noexcept;

private:
  uint8 m_data;
};

}

#endif
