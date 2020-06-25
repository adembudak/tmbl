#ifndef REG8_H
#define REG8_H

#include "../../config.h"
#include <cstddef>
#include <optional>

namespace tmbl::cpu {

// clang-format off

/*!
   A type for 8 bit registers.
   In practive used only for A(ccumulator) and F(lag) registers
   
   Register F
   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |                                                                       
   |---|---|---|---|---|---|---|---|                                                                         
   | Z | N | H | C | / | / | / | / |                                                                         
                                                                                                             
   Z:  Set to 1 when the result of an operation is 0; otherwise reset.
   N:  Set to 1 following execution of the substruction instruction, regardless of the  result.
   H:  Set to 1 when an operation results in carrying from or borrowing to bit 3.
   CY:  Set to 1 when an operation results in carrying from or borrowing to bit 7.
                                                                                                            
   @see https://ia801906.us.archive.org/19/items/GameBoyProgManVer1.1/GameBoyProgManVer1.1.pdf page 94-95
*/

// clang-format on

class reg8 final {
public:
  reg8 &operator=(const u8 n) noexcept;
  reg8 &operator+=(const u8 n) noexcept;
  reg8 &operator-=(const u8 n) noexcept;

  bool test(std::size_t pos) const;

  void Z(cflag val_) noexcept;
  void N(cflag val_) noexcept;
  void H(cflag val_) noexcept;
  void C(cflag val_) noexcept;

  cflag Z() const noexcept;
  cflag N() const noexcept;
  cflag H() const noexcept;
  cflag C() const noexcept;

  u8 loNibble() const noexcept;
  u8 hiNibble() const noexcept;

  u8 lsb() const noexcept;
  u8 msb() const noexcept;

  flag bit0(std::optional<flag> f = std::nullopt) noexcept;
  flag bit1(std::optional<flag> f = std::nullopt) noexcept;
  flag bit2(std::optional<flag> f = std::nullopt) noexcept;
  flag bit3(std::optional<flag> f = std::nullopt) noexcept;

  [[maybe_unused]] flag bit4(std::optional<flag> f = std::nullopt) noexcept;

  flag bit5(std::optional<flag> f = std::nullopt) noexcept;
  flag bit6(std::optional<flag> f = std::nullopt) noexcept;
  flag bit7(std::optional<flag> f = std::nullopt) noexcept;

  static u8 min() noexcept;
  static u8 max() noexcept;

  [[nodiscard]] u8 value() const noexcept;

private:
  mutable u8 m_data = 0b0000'0000U;

  static constexpr u8 min_u8_val = 0b0000'0000;
  static constexpr u8 max_u8_val = 0b1111'1111;

  static constexpr u8 zeroed_upper_byte_mask = 0b0000'1111U;
  static constexpr u8 zeroed_lower_byte_mask = 0b1111'0000U;
};

[[nodiscard]] reg8 operator+(const reg8 r1, const reg8 r2);
[[nodiscard]] reg8 operator-(const reg8 r1, const reg8 r2);
[[nodiscard]] reg8 operator&(const reg8 r1, const reg8 r2);
[[nodiscard]] reg8 operator|(const reg8 r1, const reg8 r2);
[[nodiscard]] reg8 operator^(const reg8 r1, const reg8 r2);
}

#endif

