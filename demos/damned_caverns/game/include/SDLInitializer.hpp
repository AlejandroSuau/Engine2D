#pragma once

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdexcept>

// Se crean las clases respectivas para poder inicializar los subsistemas utilizados de SDL2 para que
// cuando se destruyan, liberen los recursos reservados (Patrón RAII).

// Initialize SDL.
class SDLInitializer {
public:
    SDLInitializer() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("Failed to init SDL: %s", SDL_GetError());
            throw std::runtime_error("Failed to init SDL");
        }
    }

    ~SDLInitializer() { SDL_Quit(); }
};

// Initialize SDL Image.
class SDLImageInitializer {
public:
    SDLImageInitializer() {
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            SDL_Log("Failed to init SDL: %s", SDL_GetError());
            throw std::runtime_error("Failed to init Image SDL");
        }
    }
    ~SDLImageInitializer() { IMG_Quit(); }
};

// Initialize SDL Mixer.
class SDLMixerInitializer {
public:
    SDLMixerInitializer() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("Failed to init SDL Mixer: %s", Mix_GetError());
            throw std::runtime_error("Failed to init SDL Mixer");
        }
    }

    ~SDLMixerInitializer() { Mix_CloseAudio(); }
};
