#include "sdl_initializer/SDLInitializer.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>

namespace Engine2D::SDLInitializer {

Main::Main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        throw std::runtime_error("Failed to init SDL");
    }
}

Main::~Main() {
    SDL_Quit();
}

}