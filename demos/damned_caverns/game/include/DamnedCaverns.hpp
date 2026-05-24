#pragma once

#include <SDL2/SDL.h>

#include "SDLInitializer.hpp"
#include "Constants.hpp"
#include "Grid.hpp"

#include <memory>
#include <array>

class DamnedCaverns {
public:
    DamnedCaverns(int window_width, int window_height);
    ~DamnedCaverns();

    void Run();

private:
    std::unique_ptr<SDLInitializer> sdl_initializer_;
    std::unique_ptr<SDLMixerInitializer> sdl_initializer_mixer_;

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;

    int window_width_;
    int window_height_;

    float elapsed_time_ = 0.0f;
    bool is_running_;

    Grid grid_;

    void CreateWindow();
    void SetMapData();
    void Init();

    void CoreLoop();
    void Update(float dt);
    void Render();
    void HandleEvents();
    void Shutdown();    
};