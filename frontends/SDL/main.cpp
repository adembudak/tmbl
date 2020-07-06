#include "tmbl/cpu/cpu.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/bus/bus.h"

#include <SDL2/SDL.h>

int main(int /*argc*/, char * /*argv*/[]) {
  using namespace tmbl;

  SDL_Window *window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160,
                                        144, SDL_WINDOW_SHOWN);

  cartridge cart("/home/adem/Projects/tombul/build/frontends/SDL/tetris.gb");

  bus::get().plug(cart);

  SDL_SetWindowTitle(window, bus::get().title().c_str());

  cpu c;
  c.run();

  SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
  SDL_bool done = SDL_FALSE;
  SDL_Event event;

  while (!done) {
    while (!done && SDL_PollEvent(&event)) {
      switch (event.type) {

        case SDL_QUIT:
          done = SDL_TRUE;
          break;

        case SDL_DROPFILE:
          char *cartridge_file = event.drop.file;

          SDL_free(cartridge_file);
          break;
      }
    }
    SDL_Delay(1);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
