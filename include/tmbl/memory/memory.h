#ifndef MEMORY_H
#define MEMORY_H

#include "../config.h"
#include <array>

namespace tmbl::memory {

class memory final {
public:
  [[nodiscard]] bool dumpROM(const char *romfile);
  [[nodiscard]] byte *data() const noexcept;

private:
  mutable std::array<byte, 64 * 1024> ram{};
};

static_assert(is_regular_v<memory>);
}

#endif
