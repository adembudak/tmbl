#include "tmbl/cartridge/metadata/pakInfo.h"
#include "tmbl/cartridge/metadata/codes.h"
#include <algorithm>
#include <ostream>

namespace tmbl::metadata {

pakInfo::pakInfo(std::vector<byte> &&vec) : cartridge_header(std::move(vec)) {

  nintendo_logo_check = std::equal(begin(nintendo_logo), end(nintendo_logo),
                                   &cartridge_header[0x104], &cartridge_header[0x133 + 1]);

  title = std::string(&cartridge_header[0x0134], &cartridge_header[0x013E + 1]);

  manufacturer_code = std::string(&cartridge_header[0x013F], &cartridge_header[0x0142 + 1]);

  cgb_flag = cartridge_header[0x0143];

  new_licensee_code = ((cartridge_header[0x0144] & 0x0F) << 8) | (cartridge_header[0x0145] & 0x0F);

  // set sgb_flag

  // clang-format off
  sgb_flag =                cartridge_header[0x0146];
  cartridge_type =          cartridge_header[0x0147];
  rom_size =                cartridge_header[0x0148];
  ram_size =                cartridge_header[0x0149];
  destination_code =        cartridge_header[0x014A];
  old_licensee_code =       cartridge_header[0x014B];
  mask_rom_version_number = cartridge_header[0x014C];
  // clang-format on
}

std::ostream &operator<<(std::ostream &os, const pakInfo &pak) {

  os << std::string(50, '~') << '\n';

  os << "nintendo logo check: " << std::boolalpha << pak.nintendo_logo_check << '\n';
  os << "title: " << pak.title << '\n';
  os << "manufacturer code: " << pak.manufacturer_code << '\n'; // for "newer" cartridges
  os << "CGB support: [00h: no, 80h: compatible, C0h: exclusive] " << std::hex << pak.cgb_flag
     << '\n';

  os << "new licensee code: " << std::hex << pak.new_licensee_code << ':'
     << new_licensee[pak.new_licensee_code] << '\n';
  os << "SGB support: [00h: incompatible, 03h: uses sgb features] " << std::hex << pak.sgb_flag
     << '\n';

  // print out sgb support

  os << "cartridge type: " << cartridge_types[pak.cartridge_type] << '\n';
  os << "rom size: " << rom_sizes[pak.rom_size] << '\n';
  os << "ram size: " << rom_sizes[pak.ram_size] << '\n';
  os << "destination code [0x00: JP, 0x01: all other]: " << pak.destination_code << '\n';
  os << "old licensee code: " << std::hex << pak.old_licensee_code << ':'
     << old_licensee[pak.old_licensee_code] << '\n';

  os << "mask rom version number: " << pak.mask_rom_version_number << '\n';

  return os;
}

}
