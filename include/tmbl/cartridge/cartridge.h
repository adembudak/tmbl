#ifndef CARTRIDGE_h
#define CARTRIDGE_h

#include "../config.h"
#include "mbc/mbc.h"
#include "mbc/mbc1/mbc1.h"
#include <filesystem>
#include <string>
#include <variant>
#include <vector>

namespace tmbl {

class cartridge final {
public:
  explicit cartridge(const std::filesystem::path &p);

  const std::string title() const noexcept;
  std::string manufacturer() noexcept;

  bool CGB() const noexcept;
  std::string newLicenseCode() noexcept;
  bool SGB() const noexcept;

  void cartType() noexcept;
  void cartRom() noexcept;
  void cartRam() noexcept;
  void destinationCode() noexcept;
  std::string oldLicenseCode() noexcept;
  void romVersion() noexcept;
  byte headerChecksum() const noexcept;

  std::vector<byte> rom{};
  std::vector<byte> external_ram{};

private:
  bool has_ram = false;
  bool has_battery = false;
  bool has_timer = false;
  bool has_rumble = false;

  bool ram_disabled = false;

  bool cgb_enabled = false;
  bool sgb_enabled = false;

  int rom_size = 0;
  int ram_size = 0;

  int rom_banks = 0;
  int ram_banks = 0;

  std::string destination_code{};
  byte rom_version{};

  std::shared_ptr<mbc> mbc_type;
};

} // namespace tmbl

#endif
