#ifndef FLAGS_H
#define FLAGS_H

#include "tmbl/config.h"

namespace tmbl {

class flags final {
public:
  void z(flag f);
  void n(flag f);
  void h(flag f);
  void c(flag f);

  [[nodiscard]] cflag z() const noexcept;
  [[nodiscard]] cflag n() const noexcept;
  [[nodiscard]] cflag h() const noexcept;
  [[nodiscard]] cflag c() const noexcept;

  flags &operator=(const byte b);

  uint8 value() const noexcept;

private:
  flag m_z = false;
  flag m_n = false;
  flag m_h = false;
  flag m_c = false;
};

}

#endif
