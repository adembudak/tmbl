#include "tmbl/cartridge/cartridge.h"
#include "tmbl/io/registers.h"
#include "tmbl/io/interrupts/interrupts.h"
#include "tmbl/io/joypad/joypad.h"
#include "tmbl/builtin/builtin.h"
#include "tmbl/ppu/ppu.h"
#include "tmbl/bus/bus.h"
#include "tmbl/cpu/cpu.h"
#include "tmbl/dma/dma.h"

#include <SDL2/SDL.h>

#include <filesystem>
#include <functional>
#include <cstdio>

class gameboy final {
public:
  gameboy() {
    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    sdl_window = SDL_CreateWindow(/*title=*/"", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, tmbl::screenWidth, tmbl::screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGB555, SDL_TEXTUREACCESS_STREAMING, tmbl::screenWidth, tmbl::screenHeight);
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

  void handleKeyboard(const SDL_KeyboardEvent &e, bool &status) {
    switch (e.type) {
      case SDL_KEYDOWN:
        switch (e.keysym.sym) {
          case SDLK_w:
            m_joypad.dpadUp(tmbl::button::Pressed);
            break;

          case SDLK_d:
            m_joypad.dpadRight(tmbl::button::Pressed);
            break;

          case SDLK_s:
            m_joypad.dpadDown(tmbl::button::Pressed);
            break;

          case SDLK_a:
            m_joypad.dpadLeft(tmbl::button::Pressed);
            break;

          case SDLK_k:
            m_joypad.a(tmbl::button::Pressed);
            break;

          case SDLK_j:
            m_joypad.b(tmbl::button::Pressed);
            break;

          case SDLK_RETURN:
            m_joypad.select(tmbl::button::Pressed);
            break;

          case SDLK_SPACE:
            m_joypad.start(tmbl::button::Pressed);
            break;

          case SDLK_ESCAPE:
            status = false;
            break;
        }
        break;

      case SDL_KEYUP:
        switch (e.keysym.sym) {
          case SDLK_w:
            m_joypad.dpadUp(tmbl::button::Released);
            break;

          case SDLK_d:
            m_joypad.dpadRight(tmbl::button::Released);
            break;

          case SDLK_s:
            m_joypad.dpadDown(tmbl::button::Released);
            break;

          case SDLK_a:
            m_joypad.dpadLeft(tmbl::button::Released);
            break;

          case SDLK_k:
            m_joypad.a(tmbl::button::Released);
            break;

          case SDLK_j:
            m_joypad.b(tmbl::button::Released);
            break;

          case SDLK_RETURN:
            m_joypad.select(tmbl::button::Released);
            break;

          case SDLK_SPACE:
            m_joypad.start(tmbl::button::Released);
            break;
        }
        break;
    }
  }

  bool processInput(SDL_Event &event) {
    bool status = true;
    while (SDL_PollEvent(&event)) {
      handleQuit(event.quit, status);
      handleKeyboard(event.key, status);
    }

    return status;
  }

  void plugCart(const std::filesystem::path &p) {
    m_cart.init(p);
  }

  void draw(tmbl::ppu::frame &framebuffer) {
    SDL_UpdateTexture(sdl_texture, /*rect=*/nullptr, framebuffer.data(), tmbl::screenWidth * sizeof(tmbl::ppu::color));
  }

  void run() {
    SDL_Event event;
    for (bool running = true; running; /**/) {
      running = processInput(event);
      m_cpu.run();
      m_ppu.update(std::bind(&gameboy::draw, this, std::placeholders::_1));
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

  tmbl::joypad m_joypad{m_regs};
  tmbl::builtin m_builtin{m_regs, m_cart};
  tmbl::ppu m_ppu{m_regs, m_cart, m_intr};
  tmbl::dma m_dma{m_ppu, m_regs, m_builtin, m_cart};
  tmbl::bus m_bus{m_cart, m_regs, m_intr, m_builtin, m_ppu, m_dma};
  tmbl::cpu m_cpu{m_bus, m_regs, m_intr};
};

int main(int argc, const char *const argv[]) {
  if (argc > 1) {
    if (const auto cartridge = std::filesystem::path{argv[1]}; std::filesystem::exists(cartridge)) {
      gameboy attaboy;
      attaboy.plugCart(cartridge);
      attaboy.run();
    } else {
      std::printf("File not exist: %s\n", cartridge.string().c_str());
      return 1;
    }
  } else {
    std::puts("Usage: tombul [ROM file]");
    return 0;
  }
}
