#include "sdl_initializer/SDLMixerInitializer.hpp"

#include <SDL2/SDL.h>
#include <SDL_mixer.h>

#include <stdexcept>

namespace Engine2D::SDLInitializer {

Mixer::Mixer() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Failed to init SDL Mixer: %s", Mix_GetError());
        throw std::runtime_error("Failed to init SDL Mixer");
    }
}

Mixer::~Mixer() {
    Mix_CloseAudio();
}

}
