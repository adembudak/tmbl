#ifndef METADATA_H
#define METADATA_H

#include "tmbl/config.h"

#include <string>
#include <vector>

namespace tmbl::metadata {

class pakInfo {
public:
  pakInfo(std::vector<byte> &&vec);
  friend std::ostream &operator<<(std::ostream &os, const pakInfo &pak);

private:
  bool nintendo_logo_check = false;
  std::string title;
  std::string manufacturer_code;
  byte cgb_flag;
  byte new_licensee_code;
  byte sgb_flag;
  byte cartridge_type;
  byte rom_size;
  byte ram_size;
  byte destination_code;
  byte old_licensee_code;
  byte mask_rom_version_number;

  std::vector<byte> cartridge_header;
};

}

#endif
