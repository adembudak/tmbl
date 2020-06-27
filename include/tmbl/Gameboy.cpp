#include "Gameboy.hpp"

namespace tmbl {

void Gameboy::plug(const std::filesystem::path &cart) {}

void Gameboy::saveState() {}
void Gameboy::loadState() {}
void Gameboy::pause() {}
void Gameboy::run() { c.run(); }

}
