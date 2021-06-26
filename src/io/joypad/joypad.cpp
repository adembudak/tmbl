#include "tmbl/io/joypad/joypad.h"
#include "tmbl/io/registers.h"

namespace tmbl {
joypad::joypad(registers &regs_) : m_regs(regs_), P1(regs_.getAt(0xFF00)) {}

void joypad::dpadUp(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0010'0001;
      break;

    case button::Released:
      P1 &= 0b1101'1110;
      break;
  }
}

void joypad::dpadRight(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0010'0100;
      break;

    case button::Released:
      P1 &= 0b1101'1011;
      break;
  }
}

void joypad::dpadDown(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0010'1000;
      break;

    case button::Released:
      P1 &= 0b1101'0111;
      break;
  }
}

void joypad::dpadLeft(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0001'0010;
      break;

    case button::Released:
      P1 &= 0b1110'1101;
      break;
  }
}

void joypad::select(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0010'0100;
      break;

    case button::Released:
      P1 &= 0b1101'1011;
      break;
  }
}

void joypad::start(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0010'1000;
      break;

    case button::Released:
      P1 &= 0b1101'0111;
      break;
  }
}

void joypad::a(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0010'0001;
      break;

    case button::Released:
      P1 &= 0b1101'1110;
      break;
  }
}

void joypad::b(const button b) noexcept {
  switch (b) {
    case button::Pressed:
      P1 |= 0b0010'0010;
      break;

    case button::Released:
      P1 &= 0b1101'1101;
      break;
  }
}

}
