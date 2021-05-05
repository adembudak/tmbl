#ifndef MBC2
#define MBC2

#include "tmbl/config.h"
#include <vector>

namespace tmbl {
class mbc2 final {
public:
  mbc2() = default;
  explicit mbc2(std::vector<byte> &&rom);

  byte read(const std::size_t index) noexcept;
  void write(const std::size_t index, const byte val) noexcept;

private:
  std::vector<byte> m_xram{};

  flag xram_access_enabled = false; // ramg, ram gate register
  byte romb = 1;                    // rom bank register, 1 by default

  std::vector<byte> m_rom{};
};

}
#endif
