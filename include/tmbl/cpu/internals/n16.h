#ifndef N16_H
#define N16_H

#include "../../config.h"

namespace tmbl {

class n16 final {
public:
  n16(const uint16 n);
  uint16 value() const noexcept;

private:
  uint16 m_data;
};

}

#endif
