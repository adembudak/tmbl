#include "gameboy.h"

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc > 1) {
    gameboy myboy;
    myboy.plugCart(argv[1]);

    for (;;) {
      myboy.run();
    }
  } else {
    std::cerr << "Pass a ROM file.\n";
  }
}
