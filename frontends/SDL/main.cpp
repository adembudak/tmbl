#include "tmbl/Gameboy.hpp"

#include <SDL2/SDL.h>
#include <iostream>

void echo(const char *message) { std::cout << message << '\n'; }

int main(int /*argc*/, char * /*argv*/[]) {
  tmbl::Gameboy machine;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_Window *window = SDL_CreateWindow(/*title=*/"Something", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 160 * 5, 144 * 5, SDL_WINDOW_SHOWN);

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
          char *droppped_file = event.drop.file;

          machine.init(droppped_file);

          SDL_free(droppped_file);
          break;
      }

      switch (event.key.state) {

        case SDL_PRESSED:
          switch (event.key.keysym.sym) {

              // TODO implement controller
            case SDLK_a:
              [[fallthrough]];
            case SDLK_LEFT:
              echo("A or left pressed");
              break;

            case SDLK_w:
              [[fallthrough]];
            case SDLK_UP:
              echo("w or up pressed");
              break;

            case SDLK_d:
              [[fallthrough]];
            case SDLK_RIGHT:
              echo("d or right pressed");
              break;

            case SDLK_s:
              [[fallthrough]];
            case SDLK_DOWN:
              echo("s or down pressed");
              break;

            case SDLK_SPACE:
              echo("space pressed");
              break;
              break;

            case SDLK_ESCAPE:
              echo("esc pressed");
              break;
          }

        case SDL_RELEASED:
          switch (event.key.keysym.sym) {

              // TODO implement controller
            case SDLK_a:
              [[fallthrough]];
            case SDLK_LEFT:
              echo("A or left released");
              break;
          }
      }
    }
    //    SDL_Delay(0);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
