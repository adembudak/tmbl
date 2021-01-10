#include "tmbl/bus/bus.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/cpu/cpu.h"
#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/registers.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/io/interrupts/interrupts.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <filesystem>

class gameboy final {
public:
  gameboy() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    sdl_window = SDL_CreateWindow("", 50, 30, screenWidth, screenHeight, 0);
    sdl_renderer =
        SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    sdl_texture =
        SDL_CreateTexture(sdl_renderer, 0, SDL_TEXTUREACCESS_STATIC, screenWidth, screenHeight);
  }

  ~gameboy() {
    SDL_DestroyTexture(sdl_texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  }

  bool processInput(SDL_Event &event) {
    bool status = true;

    while (SDL_PollEvent(&event)) {

      switch (event.quit.type) {
        case SDL_QUIT:
          status = false;
          break;
        default:
          break;
      }

      switch (event.key.type) {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_j:
              /*handle event*/
              break;
            default:
              break;
          }
          break;

        case SDL_KEYUP:
          switch (event.key.keysym.sym) {
            case SDLK_j:
              /*handle event*/
              break;
            default:
              break;
          }
          break;
      }
    }

    return status;
  }

  void plugCart(std::filesystem::path p) { m_cart.init(p); }

  void run() {
    SDL_Event event;
    for (bool running = true; running; /**/) {
      running = processInput(event);
      m_cpu.run();
      //      m_ppu.render(drawFunc);
    }
  }

private:
  static constexpr std::size_t screenWidth = 160;
  static constexpr std::size_t screenHeight = 144;

  SDL_Texture *sdl_texture;
  SDL_Renderer *sdl_renderer;
  SDL_Window *sdl_window;

private:
  tmbl::cartridge m_cart;
  tmbl::registers m_regs;
  tmbl::interrupts m_intr;
  tmbl::builtin m_builtin;

  tmbl::ppu m_ppu{m_regs, m_cart, m_intr};
  tmbl::bus m_bus{m_cart, m_regs, m_intr, m_builtin, m_ppu};
  tmbl::cpu m_cpu{m_bus, m_regs, m_intr};
};

int main(int argc, char *argv[]) {
  if (argc > 1) {
    gameboy myboy;
    myboy.plugCart("argv[1]");
    myboy.run();
  } else {
    std::cout << "Usage: tombul [ROM file]\n";
  }
}

