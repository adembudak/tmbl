#include "gameboy.h"

void gameboy::plugCart(std::filesystem::path p) { p_cart->init(p); }
void gameboy::run() { m_cpu.run(); }
