#ifndef CPU_H
#define CPU_H

#include "registers/reg16.h"
#include "registers/reg8.h"
#include "../config.h"

namespace tmbl::cpu {

template <typename T> struct is_register : std::false_type {};
template <> struct is_register<reg8> : std::true_type {};
template <> struct is_register<reg16> : std::true_type {};

template <typename T> static constexpr bool is_register_v = is_register<T>::value;
template <typename T> concept Register = is_register_v<T>;

template <typename T> struct is_intermediate_data : std::false_type {};
template <> struct is_intermediate_data<uint8> : std::true_type {};
template <> struct is_intermediate_data<uint16> : std::true_type {};

template <typename T>
static constexpr bool is_intermidate_data_v = is_intermediate_data<T>::value;
template <typename T> concept IntermediateData = is_intermidate_data_v<T>;

class cpu final {
public:
  void LD(Register auto &r1, Register auto &r2, uint8 CY) noexcept {
    r1 = r2;
    PC += CY;
  }

  void LD(Register auto &r, IntermediateData auto n, uint8 CY) noexcept {
    r = n;
    PC += CY;
  }

  void NOP() noexcept;
  void PUSH(reg16 r);
  void POP(reg16 r);

  void LDHL(uint8 n);
  void LD(reg16 r) noexcept;

  void run();

private:
  byte fetch() { return {}; }
  void decode();
  void execute();

private:
  mutable reg16 PC;
  mutable reg16 SP;

  mutable reg8 A;
  mutable reg8 F;

  mutable reg16 BC;
  mutable reg16 DE;
  mutable reg16 HL;
};

static_assert(is_regular_v<cpu>);
}

#endif

