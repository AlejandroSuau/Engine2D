#pragma once

#include <SDL2/SDL.h>

#include "game/pacman/Entity.hpp"
#include "game/pacman/IChaseStrategy.hpp"
#include "grid/GridMover.hpp"
#include "utils/Types.hpp"

#include <string>
#include <memory>

enum class EGhostMode {
    HOUSING,
    CHASING,
    SCATTERING,
    FRIGHTENING,
};

class Ghost : public Entity {
public:
    Ghost(
        std::string name,
        Engine2D::Coords_t scattered_pos,
        SDL_Color color);

private:
    const std::string name_;
    const Engine2D::Coords_t scattered_pos_;
    const SDL_Color color_;
    EGhostMode mode_ { EGhostMode::HOUSING };

    std::unique_ptr<Engine2D::GridMover> mover_;
    std::unique_ptr<IChaseStrategy> chase_strategy_;
};
