#ifndef MBC_H
#define MBC_H
#include <memory>

namespace tmbl::cartridge {

class cartridge;

class mbc {
public:
  explicit mbc(std::shared_ptr<cartridge> cartridge) : m_cartidge(cartridge) {}

protected:
  std::shared_ptr<cartridge> m_cartidge;
};

} // namespace tmbl::cartridge

#endif
