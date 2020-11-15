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

  enum class cgb_support_code : byte {
    CGB_Incompatible = 0x00,
    CGB_Compatible = 0x80,
    CGB_Exclusive = 0xC0
  };

  enum class sgb_support_code : byte {
    SGB_Incompatible = 0x00,
    Uses_SGB_features = 0x03,
  };

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
