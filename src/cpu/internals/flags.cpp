#include "tmbl/cpu/internals/flags.h"

namespace tmbl {

void flags::Z(flag f) { m_Z = f; }
void flags::N(flag f) { m_N = f; }
void flags::H(flag f) { m_H = f; }
void flags::C(flag f) { m_C = f; }

cflag flags::Z() const noexcept { return m_Z; }
cflag flags::N() const noexcept { return m_N; }
cflag flags::H() const noexcept { return m_H; }
cflag flags::C() const noexcept { return m_C; }

}
