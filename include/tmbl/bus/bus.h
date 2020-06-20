#ifndef BUS_H
#define BUS_H

#include "../config.h"
#include "../memory/memory.h"

namespace tmbl::cpu {
class reg8;
class reg16;
}

namespace tmbl::bus {

class bus final {
public:
  [[nodiscard]] byte read(const cpu::reg8 r);
  [[nodiscard]] byte read(const cpu::reg16 rr);
  [[nodiscard]] byte read(const u8 n);
  [[nodiscard]] byte read(const u16 nn);

  void write(const cpu::reg8 r, const u8 n);
  void write(const cpu::reg16 rr, const cpu::reg8 r);
  void write(const cpu::reg16 rr, const u16 nn);
  void write(const u16 nn, const cpu::reg8 r);
  void write(const u16 nn, const byte b);
  void write(const cpu::reg16 rr, const byte b);
  void write(const cpu::reg16 rr1, const cpu::reg16 rr2);

private:
  memory::memory m;
};

}

#endif

