#include "../../config.h"

namespace tmbl {

class n8 final {
public:
  n8(const uint8 n);
  uint8 value() const noexcept;

private:
  uint8 m_data;
};

}
