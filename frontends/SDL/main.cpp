#include "tmbl/Gameboy.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include <string>

int main(int argc, char *argv[]) {

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  std::string title = "gametitle here";

  SDL_Window *window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

  SDL_DestroyWindow(window);
  SDL_Quit();
}
