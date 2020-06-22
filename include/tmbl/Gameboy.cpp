#include "Gameboy.hpp"

namespace tmbl {

void Gameboy::init(const std::filesystem::path &p){
	game.load(p);
}

void Gameboy::saveState() {}
void Gameboy::loadState() {}
void Gameboy::pause() {}

void Gameboy::run() {
	c.run();
}

}
