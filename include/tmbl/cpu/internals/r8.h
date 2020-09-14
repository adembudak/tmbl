#include "../../config.h"

namespace tmbl {

class n8;

class r8 final {
public:
  //  r8 &operator=(const r8 n); //Spe. Mem. Implicit,  ADC A, r8
  r8 &operator=(const byte b); // ADC A, [HL]
  r8 &operator=(const n8 n);   // ADC A, n8

  uint8 value() const noexcept;

private:
  uint8 m_data = 0;
};

}
