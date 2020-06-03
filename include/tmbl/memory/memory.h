#ifndef MEMORY_H
#define MEMORY_H

#include "../config.h"
#include "../cpu/registers/reg16.h"
#include "../cpu/registers/reg8.h"
#include <array>

namespace tmbl::memory {

class memory final {
public:
  [[nodiscard]] bool dumpROM(const char *romfile);
  [[nodiscard]] byte *data() const noexcept;

  [[nodiscard]] byte &operator[](std::size_t index);
  [[nodiscard]] byte &operator[](cpu::reg8 r);
  [[nodiscard]] byte &operator[](cpu::reg16 rr);

  [[nodiscard]] byte &operator[](u8 n);
  [[nodiscard]] byte &operator[](u16 nn);

private:
  mutable std::array<byte, 64 * 1024> ram{};
};

}

#endif
