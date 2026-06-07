#pragma once

#include <SDL2/SDL.h>

#include "SDLInitializer.hpp"
#include "Constants.hpp"
#include "Grid.hpp"
#include "AudioManager.hpp"

#include <memory>
#include <algorithm>
#include <array>

// Player
struct Player {
    Vec2<int> direction_; // El jugador siempre tendrá una dirección
    ColRow_t colrow_; // Posición top left del cuadrado
    
    bool TryMove(const Grid& grid, int amount) {
        const Vec2<int> next_colrow {
            colrow_.x + direction_.x * amount,
            colrow_.y + direction_.y * amount
        };

        const auto next_position = grid.ColRowToCoords(next_colrow);

        if (grid.AreCoordsWalkable(next_position)) {
            colrow_ = next_colrow;
            return true;
        } else {
            return false;
        }
    }

    bool MoveForward(const Grid& grid) {
        return TryMove(grid, 1);
    }

    bool MoveBackward(const Grid& grid) {
        return TryMove(grid, -1);
    }

    void RotateLeft() {
        direction_ = { direction_.y, -direction_.x };
    }

    void RotateRight() {
        direction_ = { -direction_.y, direction_.x };
    }
};

// Monsters
struct Monster {
    ColRow_t colrow_;
};

// Game State
enum class GameState {
    Playing,
    Victory,
    Defeat
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

    AudioManager audio_manager_;
    GameState game_state_;
    Grid grid_;
    Player player_;
    Monster monster_;
    
    int monster_snore_channel_ = -1;
    int waterfall_channel_ = -1;

    bool show_visual_helper_ = false;

    void ResetGame();
    void StartAmbientLoops();

    void CreateWindow();
    void SetMapData();
    void Init();

    void CoreLoop();
    void Update(float dt);
    void Render();
    void HandleEvents();
    void Shutdown();    

    void RenderVisualHelper();
    
    void MoveMonsterRandomly();
    void TriggerVictory();
    void TriggerDefeat();
    void CheckGameOver();
};