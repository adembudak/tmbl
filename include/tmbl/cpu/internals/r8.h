#include "../../config.h"

namespace tmbl {
class n8;

class r8 final {
public:
  r8() = default;
  explicit r8(const uint8 val);

  //  r8 &operator=(const r8 n); //Spe. Mem. Implicit,  ADC A, r8
  r8 &operator=(const byte b); // ADC A, [HL]
  r8 &operator=(const n8 n);   // ADC A, n8

  uint8 loNibble() const noexcept;
  uint8 hiNibble() const noexcept;

  uint8 value() const noexcept;

private:
  uint8 m_data = 0;
};

r8 operator+(const r8 l, const r8 r);
uint8 operator+(const r8 r, const uint8 u);
bool operator==(const r8 r, uint8 u);

}
