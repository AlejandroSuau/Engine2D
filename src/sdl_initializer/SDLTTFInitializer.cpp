#include "sdl_initializer/SDLTTFInitializer.hpp"

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <stdexcept>

namespace Engine2D::SDLInitializer {

TTF::TTF() {
    if (TTF_Init() == -1) {
        SDL_Log("Failed to init SDL TTF: %s", SDL_GetError());
        throw std::runtime_error("Failed to init SDL TTF");
    }
}
    
TTF::~TTF() {
    TTF_Quit();
}

}
