#include "../../config.h"

namespace tmbl {

class flags final {
public:
  void Z(flag f);
  void N(flag f);
  void H(flag f);
  void C(flag f);

private:
  flag m_Z = false;
  flag m_N = false;
  flag m_H = false;
  flag m_C = false;
};

}

