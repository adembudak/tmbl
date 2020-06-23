#ifndef MBC0_H
#define MBC0_H

#include <utility>

#include "../mbc.h"

namespace tmbl::cartridge {

class mbc0 final : public mbc {
public:
  explicit mbc0(std::shared_ptr<cartridge> cartridge)
      : mbc(std::move(cartridge)) {}

private:
};

} // namespace tmbl::cartridge

#endif
