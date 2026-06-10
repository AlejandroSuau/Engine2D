#pragma once

#include <SDL2/SDL.h>

#include "SDLInitializer.hpp"

#include <memory>

class PetRescueSaga {
public:
    PetRescueSaga(int window_width, int window_height);
    ~PetRescueSaga();

    void Run();

private:
    std::unique_ptr<SDLInitializer> sdl_initializer_;

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;

    int window_width_;
    int window_height_;

    float elapsed_time_ = 0.0f;
    bool is_running_;

    void ResetGame();

    void CreateWindow();
    void Init();

    void CoreLoop();
    void Update(float dt);
    void Render();
    void HandleEvents();
    void Shutdown();    
    
    void CheckGameOver();
};