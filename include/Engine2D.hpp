#pragma once

#include <SDL2/SDL.h>

#include "sdl_initializer/SDLInitializer.hpp"
#include "sdl_initializer/SDLImageInitializer.hpp"
#include "sdl_initializer/SDLTTFInitializer.hpp"
#include "sdl_initializer/SDLMixerInitializer.hpp"

#include "renderer/Renderer.hpp"

#include "game/IGame.hpp"

#include <memory>

namespace Engine2D {

class Engine {
public:
    Engine();
    
    void Run(std::unique_ptr<IGame> game);
    void Shutdown();

    SDL_Renderer* SDLRenderer();

    void SetCursorDisplay(bool show);

private:
    // SDL Initializers
    std::unique_ptr<SDLInitializer::Main> sdl_;
    std::unique_ptr<SDLInitializer::Image> sdl_image_;
    std::unique_ptr<SDLInitializer::Mixer> sdl_mixer_;
    std::unique_ptr<SDLInitializer::TTF> sdl_ttf_;

    // SDL window & render
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> sdl_renderer_;
    bool is_running_;

    std::unique_ptr<Renderer> renderer_;

    // Game
    std::unique_ptr<IGame> game_;

    void Init();

    void CoreLoop();
    void Update(float dt);
    void Render();
    void HandleEvents();
};

}
