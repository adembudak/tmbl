#pragma once
#include "../config.h"
#include <array>

namespace tmbl {

class memory {

  [[nodiscard]] bool dumpROM(const char *romfile);
  byte data() const noexcept;

private:
  std::array<byte, 32 * 1024> ram;
};

} // namespace tbl
