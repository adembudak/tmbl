#include "tmbl/config.h"

namespace tmbl {

class registers;

enum class button : uint8 { Released, Pressed };

class joypad final {
public:
  explicit joypad(registers &regs_);

  void dpadUp(const button b) noexcept;
  void dpadRight(const button b) noexcept;
  void dpadDown(const button b) noexcept;
  void dpadLeft(const button b) noexcept;

  void select(const button b) noexcept;
  void start(const button b) noexcept;

  void a(const button b) noexcept;
  void b(const button b) noexcept;

private:
  byte &P1;
  registers &m_regs;
};

}
