#include "memory.h"

namespace tmbl::memory {

[[nodiscard]] bool memory::dumpROM(const char *romfile) { return {}; }
[[nodiscard]] byte *memory::data() const noexcept { return ram.data(); }

}
