#ifndef E8_H
#define E8_H

#include "../../config.h"

namespace tmbl {

class e8 final {
public:
  e8(const int8 i);
  int8 loNibble() const noexcept;
  int8 hiNibble() const noexcept;

  int8 value() const noexcept;

private:
  int8 m_data;
};

}

#endif

