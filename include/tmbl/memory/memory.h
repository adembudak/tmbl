#ifndef MEMORY_H
#define MEMORY_H

#include "../config.h"
#include <array>

namespace tmbl::cpu {
class reg8;
class reg16;
}

namespace tmbl::memory {

class memory final {
public:
  [[nodiscard]] bool dumpROM(const char *romfile);
  [[nodiscard]] byte *data() const noexcept;

  [[nodiscard]] byte &operator[](const cpu::reg8 r);
  [[nodiscard]] byte &operator[](const cpu::reg16 rr);

  [[nodiscard]] byte &operator[](const u8 n);
  [[nodiscard]] byte &operator[](const u16 nn);

private:
  mutable std::array<byte, 64 * 1024> m_data{};
};

}

#endif
