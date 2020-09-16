#include "../../config.h"

namespace tmbl {

class flags final {
public:
  void Z(flag f);
  void N(flag f);
  void H(flag f);
  void C(flag f);

  cflag Z() const noexcept;
  cflag N() const noexcept;
  cflag H() const noexcept;
  cflag C() const noexcept;

private:
  flag m_Z = false;
  flag m_N = false;
  flag m_H = false;
  flag m_C = false;
};

}

