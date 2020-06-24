#include "Gameboy.hpp"
#include "cartridge/cartridge.h"

namespace tmbl {

void Gameboy::plug(const std::filesystem::path &cart) {
  cartridge::cartridge game(cart);
  title = game.title();
}

void Gameboy::saveState() {}
void Gameboy::loadState() {}
void Gameboy::pause() {}
void Gameboy::run() { c.run(); }

}
