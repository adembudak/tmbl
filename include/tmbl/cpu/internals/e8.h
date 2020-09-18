#include "../../config.h"

namespace tmbl {

class e8 final {
public:
  e8(const int8 i);
  int8 value() const noexcept;

private:
  int8 m_data;
};

}
