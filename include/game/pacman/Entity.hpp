#pragma once

#include <SDL2/SDL.h>

#include "utils/Types.hpp"

#include "grid/GridMover.hpp"
#include "grid/GridMoverUtils.hpp"

#include "game/pacman/IStrategy.hpp"

#include <optional>
#include <memory>

namespace Engine2D {
class Grid;
}

struct Entity {
public:
    Entity(Engine2D::Coords_t pos, float dim);

    virtual ~Entity() = default;
    virtual void Update(float dt, const Engine2D::Grid& grid) = 0; // GameContext instead of Grid

    SDL_FRect GetRect() const;
    SDL_FRect GetRectCentered() const;

    Engine2D::Coords_t pos_ {}; 
    float dimensions_ {0.f};

    Engine2D::GridMover mover{{.tile_size = dimensions_, .tiles_per_sec = 10.f}};
    std::unique_ptr<IChaseStrategy> chase_strategy_ { std::make_unique<IChaseStrategyPinky>() };
};

struct Player : public Entity {

    Player(Engine2D::Coords_t pos, float dimensions);
    void Update(float dt, const Engine2D::Grid& grid) override;
};

// 1. player_pos
// 2. player_pos + 4 tiles dir (if up 4 left aswell)
// 3. from blinky to player_pos + 2 tiles dir (if up 2 left aswell) - then double it
// 4. if 8 >= tiles distance go to corner else player_pos

struct Ghost : public Entity {
    
    Ghost(Engine2D::Coords_t pos, float dimensions);
    void Update(float dt, const Engine2D::Grid& grid) override;
};