#include "tmbl/config.h"
#include "tmbl/cpu/internals/flags.h"

namespace tmbl {

void flags::z(flag f) { m_z = f; }
void flags::n(flag f) { m_n = f; }
void flags::h(flag f) { m_h = f; }
void flags::c(flag f) { m_c = f; }

cflag flags::z() const noexcept { return m_z; }
cflag flags::n() const noexcept { return m_n; }
cflag flags::h() const noexcept { return m_h; }
cflag flags::c() const noexcept { return m_c; }

}
