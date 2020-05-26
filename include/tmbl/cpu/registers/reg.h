#pragma once
#include <stdexcept>

namespace tmbl {
namespace cpu {

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using byte = char;
using flag = const bool;

// TODO: checkout forat options :h fo-table

class reg16;
/*|
| 7-0 | 7-0 |
|-----|-----|
|  A  |  F  |
|  B  |  C  |
|  D  |  E  |
|  H  |  L  |

*/

/*!
 *  A type for 8 bit registers.
 *  In practive used only for A(ccumulator) and F(lag) registers
 */
class reg8 {
public:
  reg8() = default;
  explicit reg8(const uint8 val_) noexcept { m_data = val_; }

  // clang-format off
  /*!
   * Register F
   * | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
   * |---|---|---|---|---|---|---|---|
   * | Z | N | H | C | / | / | / | / |
   *
   * Z:  Set to 1 when the result of an operation is 0; otherwise reset.
   * N:  Set to 1 following execution of the substruction instruction, regardless of the  result.
   * H:  Set to 1 when an operation results in carrying from or borrowing to bit 3.
   * CY:  Set to 1 when an operation results in carrying from or borrowing to bit 7.
   *
   * @see https://ia801906.us.archive.org/19/items/GameBoyProgManVer1.1/GameBoyProgManVer1.1.pdf page 94-95
   */
  // clang-format on

  /// Setter for those flags
  void Z(flag val_ = true) noexcept {
    val_ ? m_data |= 0b1000'0000 : m_data &= 0b0111'1111;
  }

  void N(flag val_ = true) noexcept {
    val_ ? m_data |= 0b0100'0000 : m_data &= 0b1011'1111;
  }

  void H(flag val_ = true) noexcept {
    val_ ? m_data |= 0b0010'0000 : m_data &= 0b1101'1111;
  }

  void C(flag val_ = true) noexcept {
    val_ ? m_data |= 0b0001'0000 : m_data &= 0b1110'1111;
  }

  const reg8 &operator=(const uint8 val_) {
    if (val_ < 0 || val_ > 255)
      throw std::out_of_range("8 bit register value must be in range [0, 255)");

    m_data = val_;
    return *this;
  }

  const reg8 &operator=(const reg16 r16_) {
    if (auto val = r16_.data(); val < 0 || val > 255)
      throw std::out_of_range("8 bit register value must be in range [0, 255)");

    m_data = r16_.data();
    return *this;
  }

  [[nodiscard]] const bool operator==(const uint8 val_) {
    return m_data == val_;
  }

  [[nodiscard]] const bool operator==(const reg8 r8_) {
    return m_data == r8_.data();
  }

  [[nodiscard]] const uint8 data() const noexcept { return m_data; }

private:
  uint8 m_data = 0b0000'0000;
};

/*!
 * A type for 8 bit registers used as a pair like B and C used as BC.
 * In this cases B is upper byte and C is lower byte. These bytes can be
 * accessed and assigned by the lo() and hi() member functions.
 */

class reg16 {
public:
  reg16() = default;
  explicit reg16(const reg8 r8_) noexcept { m_data = r8_.data(); }
  explicit reg16(const uint16 val_) noexcept { m_data = val_; }

  reg16 &operator=(const reg8 r8_) noexcept {
    m_data = r8_.data();
    return *this;
  }

  const reg16 &operator=(const uint16 val_) {
    if (val_ < 0 || val_ > 65'535)
      throw std::out_of_range(
          "16 bit register value must be in range [0, 65'535)");

    m_data = val_;
    return *this;
  }

  reg16 &operator+=(const std::size_t n) {
    if (data() + n > 65'535)
      throw std::overflow_error(
          "16 bit register value can hold at most 65'535");

    m_data += n;

    return *this;
  }

  void lo(const reg8 val_) {}

  [[nodiscard]] const reg8 lo() const noexcept {
    return reg8(m_data & 0b0000'0000'1111'111);
  }

  void hi(const reg8 val_) {}

  [[nodiscard]] const reg8 hi() const noexcept {
    return reg8((m_data & 0b1111'1111'0000'0000) >> 8);
  }

  [[nodiscard]] const uint16 data() const noexcept { return m_data; }

private:
  uint16 m_data = 0b0000'0000'0000'0000;
};
}

}
