#include "game/pacman/Entity.hpp"

#include "grid/Grid.hpp"

Entity::Entity(Engine2D::Coords_t pos, float dim)
    : pos_(pos), dimensions_(dim) {}

SDL_FRect Entity::GetRect() const { 
    return {pos_.x, pos_.y, dimensions_, dimensions_};
}

SDL_FRect Entity::GetRectCentered() const {
    return {pos_.x - dimensions_ / 2.f, pos_.y - dimensions_ / 2.f, dimensions_, dimensions_};
}

/** Player */

Player::Player(Engine2D::Coords_t pos, float dimensions) 
    : Entity(pos, dimensions) {}

void Player::Update(float dt, const Engine2D::Grid& grid) {
    mover.Update(dt, grid, pos_);
}


/** Ghost */

Ghost::Ghost(Engine2D::Coords_t pos, float dimensions) 
    : Entity(pos, dimensions) {}

void Ghost::Update(float dt, const Engine2D::Grid& grid) {
    auto final_target = mover.FinalTarget();
    if (final_target) {
        // Get direction to target
        mover.SetNextDirection(Engine2D::ResultDirection(pos_, *final_target));
    }
    
    mover.Update(dt, grid, pos_);
}
