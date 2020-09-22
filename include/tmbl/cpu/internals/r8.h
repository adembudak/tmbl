#include "../../config.h"

namespace tmbl {
class n8;

class r8 final {
public:
  //  r8 &operator=(const r8 n); //Spe. Mem. Implicit,  ADC A, r8
  r8 &operator=(const byte b); // ADC A, [HL]
  r8 &operator=(const n8 n);   // ADC A, n8

  uint8 loNibble() const noexcept;
  uint8 hiNibble() const noexcept;

  uint8 value() const noexcept;

  static constexpr const uint8 zero = 0;
  static constexpr const uint8 lower_mask = 0b0000'1111;
  static constexpr const uint8 upper_mask = 0b1111'0000;

  static constexpr const uint8 min = 0b0000'0000;
  static constexpr const uint8 max = 0b1111'1111;

private:
  uint8 m_data = 0;
};

r8 operator+(const r8 l, const r8 r);
uint8 operator+(const r8 r, const uint8 u);
bool operator==(const r8 r, uint8 u);

}
