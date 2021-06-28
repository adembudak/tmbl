#ifndef OBP_H
#define OBP_H

#include "tmbl/config.h"
#include "tmbl/io/registers.h"

namespace tmbl {

class obp {
public:
  explicit obp(byte &val_);
  [[nodiscard]] std::size_t operator[](const std::size_t val) const noexcept;

private:
  byte &m_value;
};

}

#endif