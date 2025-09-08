#include "sdl_initializer/SDLImageInitializer.hpp"

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include <stdexcept>

namespace Engine2D::SDLInitializer {

Image::Image() {
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("Failed to init SDL: %s", SDL_GetError());
        throw std::runtime_error("Failed to init Image SDL");
    }
}
    
Image::~Image() {
    IMG_Quit();
}

}
