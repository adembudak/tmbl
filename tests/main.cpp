#include "gameboy.h"

#include <filesystem>
#include <iostream>

namespace debug {
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
  return 0;
}
}

int main() {
  constexpr int argc = 2;
  char *argv[argc] {"", "path_to_rom"};

  debug::main(argc, argv);
}