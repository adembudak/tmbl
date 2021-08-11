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

  [[nodiscard]] flag z() const noexcept;
  [[nodiscard]] flag n() const noexcept;
  [[nodiscard]] flag h() const noexcept;
  [[nodiscard]] flag c() const noexcept;

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
