#ifndef BGP_H
#define BGP_H

#include "tmbl/config.h"
#include "tmbl/io/registers.h"

namespace tmbl {

class bgp {
public:
  explicit bgp(byte &val_);
  [[nodiscard]] std::size_t operator[](const std::size_t val) const noexcept;

private:
  byte &m_value;
};

}

#endif
