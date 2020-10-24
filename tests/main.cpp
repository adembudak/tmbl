#include "gameboy.h"

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc > 1) {
    try {
      gameboy myboy;
      myboy.plugCart(argv[1]);
      myboy.run();
    } catch (...) {
      std::cerr << "ROM is file: " << argv[1] << " is not exist.\n";
    }
  } else {
    std::cerr << "Pass a ROM file.\n";
  }
}
