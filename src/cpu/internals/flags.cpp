#include "tmbl/cpu/internals/flags.h"

namespace tmbl {

void flags::Z(flag f) { m_Z = f; }
void flags::N(flag f) { m_N = f; }
void flags::H(flag f) { m_H = f; }
void flags::C(flag f) { m_C = f; }

}
