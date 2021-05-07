#ifndef BGP_H
#define BGP_H

#include "tmbl/config.h"
#include "../../io/registers.h"

#include <array>

namespace tmbl {

class bgp {
public:
  explicit bgp(byte &val_);
  std::size_t bgPalette(const uint8 val) const noexcept;
  std::array<int, 4> bgPalette() const noexcept;

private:
  byte &m_value;
};

}

#endif
