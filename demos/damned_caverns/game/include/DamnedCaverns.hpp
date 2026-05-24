#pragma once

#include <SDL2/SDL.h>

#include "SDLInitializer.hpp"
#include "Constants.hpp"
#include "Grid.hpp"

#include <memory>
#include <algorithm>
#include <array>

// Player
struct Player {
    Vec2<float> direction_; // -1.f, 0.f, 1.f
    Coords_t position_;
    float speed_;

    void Update(float dt, const Grid& grid) {
        const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

        direction_ = {0, 0};

        if (keyboard_state[SDL_SCANCODE_W]) direction_.y -= 1.f;
        if (keyboard_state[SDL_SCANCODE_S]) direction_.y += 1.f;
        if (keyboard_state[SDL_SCANCODE_A]) direction_.x -= 1.f;
        if (keyboard_state[SDL_SCANCODE_D]) direction_.x += 1.f;

        const auto movement = direction_ * speed_ * dt;
        Coords_t next_position = position_ + movement;
        if (grid.AreCoordsWalkable(next_position)) {
            position_ = next_position;
        }
    }
};

// Game
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
    Player player_;

    void CreateWindow();
    void SetMapData();
    void Init();

    void CoreLoop();
    void Update(float dt);
    void Render();
    void HandleEvents();
    void Shutdown();    

    void RenderVisualHelper();
};