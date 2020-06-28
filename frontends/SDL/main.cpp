#include "tmbl/Gameboy.hpp"

#include <SDL2/SDL.h>
#include <iostream>

void echo(const char *message) { std::cout << message << '\n'; }

int main(int /*argc*/, char * /*argv*/[]) {
  tmbl::Gameboy machine;
  SDL_Window *window = SDL_CreateWindow(machine.title.c_str(), SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 160, 144, SDL_WINDOW_SHOWN);

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

          machine.plug(cartridge_file);
          SDL_SetWindowTitle(window, machine.title.c_str());

          SDL_free(cartridge_file);
          break;
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
