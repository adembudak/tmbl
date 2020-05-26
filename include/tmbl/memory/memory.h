#ifndef MEMORY_H
#define MEMORY_H

#include "../config.h"
#include <array>

namespace tmbl::memory {

class memory {
public:
  [[nodiscard]] bool dumpROM(const char *romfile);
  [[nodiscard]] const byte data() const noexcept;

private:
  std::array<byte, 64 * 1024> ram{};
};

}

#endif
