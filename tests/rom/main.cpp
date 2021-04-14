#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/bus/bus.h"
#include "tmbl/cpu/cpu.h"

#include <SDL2/SDL.h>

#include <filesystem>
#include <iostream>

class gameboy final {
public:
  gameboy() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    constexpr int screenWidth = 160;
    constexpr int screenHeight = 144;
    sdl_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    sdl_texture = SDL_CreateTexture(sdl_renderer, 0, SDL_TEXTUREACCESS_STATIC, screenWidth, screenHeight);
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
      // m_ppu.render(drawFunc);
    }
  }

private:
  SDL_Texture *sdl_texture = nullptr;
  SDL_Renderer *sdl_renderer = nullptr;
  SDL_Window *sdl_window = nullptr;

private:
  tmbl::cartridge m_cart;
  tmbl::registers m_regs;
  tmbl::interrupts m_intr;
  tmbl::builtin m_builtin;

  tmbl::ppu m_ppu{m_regs, m_cart, m_intr};
  tmbl::bus m_bus{m_cart, m_regs, m_intr, m_builtin, m_ppu};
  tmbl::cpu m_cpu{m_bus, m_regs, m_intr};
};

int main(int argc, const char *const argv[]) {
  if (argc > 1) {
    gameboy attaboy;
    attaboy.plugCart(std::filesystem::path{argv[1]});
    attaboy.run();
  } else {
    std::cerr << "Usage: tombul [ROM file]\n";
  }
}
