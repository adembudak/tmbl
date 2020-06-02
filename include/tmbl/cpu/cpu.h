#ifndef CPU_H
#define CPU_H

#include "registers/reg16.h"
#include "registers/reg8.h"
#include "../config.h"

namespace tmbl::cpu {

template <typename T> struct is_8bit_register : std::false_type {};
template <> struct is_8bit_register<reg8> : std::true_type {};

template <typename T> struct is_16bit_register : std::false_type {};
template <> struct is_16bit_register<reg16> : std::true_type {};

template <typename T>
concept Register = is_8bit_register<T>::value || is_16bit_register<T>::value;

template <typename T> struct is_intermediate_data : std::false_type {};
template <> struct is_intermediate_data<u8> : std::true_type {};
template <> struct is_intermediate_data<u16> : std::true_type {};

template <typename T> concept IntermediateData = is_intermediate_data<T>::value;

class cpu final {
public:
  void LD(reg8 &r, reg16 &rr, u8 CY) noexcept;
  void LD(reg16 &rr, reg8 &r, u8 CY) noexcept;
  void LD(reg16 &rr, u8 n, u8 CY) noexcept;
  // TODO: Resolve the redefinition problem
  // int dummy parameter used as a workaround
  void LD(reg8 &r1, reg8 &r2, u8 CY, int dummy) noexcept;
  void LD(reg8 &r1, reg8 &r2, u8 CY) noexcept;
  void LD(reg8 &r, u8 n, u8 CY, int dummy) noexcept;
  void LD(reg8 &r, u8 n, u8 CY) noexcept;
  void LD(u8 n, reg8 r, u8 CY) noexcept;
  void LD(reg8 &r, u16 nn, u8 CY) noexcept;
  void LD(u16 nn, reg8 &r, u8 CY) noexcept;
  void LD(u16 nn, reg16 &rr, u8 CY) noexcept;
  void LD(reg16 &rr1, reg16 &rr2, u8 CY) noexcept;
  void LD(reg16 &rr1, u16 nn, u8 CY) noexcept;

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

