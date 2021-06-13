#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/bus/bus.h"
#include "tmbl/cpu/cpu.h"

#include <SDL2/SDL.h>

#include <filesystem>
#include <cstdio>

class gameboy final {
public:
  gameboy() {
    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    sdl_window = SDL_CreateWindow(/*title=*/"", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tmbl::screenWidth, tmbl::screenHeight, SDL_WINDOW_SHOWN);
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, tmbl::screenWidth, tmbl::screenHeight);
  }

  ~gameboy() {
    SDL_DestroyTexture(sdl_texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  }

  void handleQuit(const SDL_QuitEvent &e, bool &status) {
    switch (e.type) {
      case SDL_QUIT:
        status = false;
        break;
    }
  }

  void handleKeyboard(const SDL_KeyboardEvent &e) {
    switch (e.type) {
      case SDL_KEYDOWN:
        switch (e.keysym.sym) {
          case SDLK_j:
            /*handle event*/
            break;
        }
        break;

      case SDL_KEYUP:
        switch (e.keysym.sym) {
          case SDLK_j:
            /*handle event*/
            break;
        }
        break;
    }
  }

  bool processInput(SDL_Event &event) {
    bool status = true;
    while (SDL_PollEvent(&event)) {
      handleQuit(event.quit, status);
      handleKeyboard(event.key);
    }

    return status;
  }

  void plugCart(std::filesystem::path p) {
    m_cart.init(p);
  }

  void run() {
    SDL_Event event;
    for (bool running = true; running; /**/) {
      running = processInput(event);
      m_cpu.run();
      m_ppu.update([&](const tmbl::ppu::frame &framebuffer) {
        /*
                for (const tmbl::ppu::screenline &line : framebuffer) {
                  for (const tmbl::ppu::color &color : line) {
                    if (color.r == 155) {
                      std::putchar('a');
                    } else if (color.r == 139) {
                      std::putchar('o');
                    } else if (color.r == 48) {
                      std::putchar('i');
                    } else if (color.r == 15) {
                      std::putchar('i');
                    }
                  }
                  std::putchar('\n');
                }
        */

        SDL_UpdateTexture(sdl_texture, /*rect=*/nullptr, framebuffer.data(), tmbl::screenWidth * sizeof(tmbl::ppu::color));
      });
      SDL_RenderCopy(sdl_renderer, sdl_texture, /*srcrect=*/nullptr, /*dstrect=*/nullptr);
      SDL_RenderPresent(sdl_renderer);
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

  tmbl::builtin m_builtin{m_regs, m_cart};
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
    std::puts("Usage: tombul [ROM file]");
  }
}
