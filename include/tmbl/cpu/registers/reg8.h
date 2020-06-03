#ifndef REG8_H
#define REG8_H

#include "../../config.h"
#include <cstddef>

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
  reg8 &operator=(byte b) noexcept {
    m_data = std::to_integer<int>(b);
    return *this;
  }

  void Z(flag val_ = true) noexcept;
  void N(flag val_ = true) noexcept;
  void H(flag val_ = true) noexcept;
  void C(flag val_ = true) noexcept;

  [[nodiscard]] u8 data() const noexcept;

private:
  mutable u8 m_data = 0b0000'0000;
};

}

#endif

