#ifndef MBC1_H
#define MBC1_H

#include "tmbl/config.h"
#include <vector>

#include <type_traits>

namespace tmbl {

// Based on: https://gekkio.fi/files/gb-docs/gbctr.pdf
// and: https://retrocomputing.stackexchange.com/a/11735

class mbc1 final {
public:
  mbc1() = default;
  mbc1(std::vector<byte> &&rom, std::size_t xram_size = 0);

  byte read(const std::size_t index) noexcept;
  void write(const std::size_t index, const byte val) noexcept;

  std::size_t rom_size() const noexcept;
  std::size_t rom_banks() const noexcept;

  std::size_t xram_size() const noexcept;
  std::size_t xram_banks() const noexcept;

  [[nodiscard]] bool has_xram() const noexcept;

private:
  std::vector<byte> m_xram; // external ram or s(ave)ram
  bool m_has_xram = false;  // initial assumption, no xram

  byte bank1;                       // mbc1 bank register 1
  byte bank2;                       // mbc1 bank register 2
  byte mode;                        // mbc1 mode register
  flag xram_access_enabled = false; // ramg, ram gate reg. disabled by defauilt

  std::vector<byte> m_rom;
};

static_assert(std::is_default_constructible_v<mbc1>);
static_assert(std::is_copy_constructible_v<mbc1>);
static_assert(std::is_copy_assignable_v<mbc1>);
static_assert(std::is_move_constructible_v<mbc1>);
static_assert(std::is_move_assignable_v<mbc1>);
static_assert(std::is_destructible_v<mbc1>);

}

#endif
