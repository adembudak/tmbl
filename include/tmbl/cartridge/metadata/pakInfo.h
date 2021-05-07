#ifndef METADATA_H
#define METADATA_H

#include "tmbl/config.h"

#include <string>
#include <array>

namespace tmbl::metadata {

class pakInfo {
public:
  explicit pakInfo(std::array<byte, 336_B> &&vec);
  friend std::ostream &operator<<(std::ostream &os, const pakInfo &pak);

private:
  bool nintendo_logo_check = false;
  std::string title;
  std::string manufacturer_code;
  int cgb_flag;
  int new_licensee_code;
  int sgb_flag;
  byte cartridge_type;
  byte rom_size;
  byte ram_size;
  int destination_code;
  int old_licensee_code;
  int mask_rom_version_number;

  std::array<byte, 336_B> cartridge_header;
};

}

#endif
