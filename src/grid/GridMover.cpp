#include "grid/GridMover.hpp"

#include <SDL2/SDL.h>

namespace Engine2D {

GridMover::GridMover(Params p) 
    : params_(p) {}

void GridMover::Update(float dt, const Grid& grid, Coords_t& pos) {
    const float speed = params_.tiles_per_sec * params_.tile_size;
    if (speed <= 0.f) return;

    // Set new target
    if (!target_) {
        if (queued_dir_ != EDirection::NONE) {
            dir_ = queued_dir_;
        }
        if (dir_ == EDirection::NONE) return;

        const auto v = FromDirectionToVector(dir_);
        Coords_t candidate = pos + Coords_t{params_.tile_size * v.x, params_.tile_size * v.y};
        if (!grid.AreCoordsWalkable(candidate)) {
            Stop();
            return;
        }
        target_ = candidate;
    }

    // Move
    const auto v = FromDirectionToVector(dir_);
    const float max_step = speed * dt;
    
    float remaining = 0.f;
    if (v.x != 0.f) remaining = (target_->x - pos.x) * v.x;
    else if (v.y != 0.f) remaining = (target_->y - pos.y) * v.y;

    const float step = std::min(max_step, std::max(0.f, remaining));
    pos.x += v.x * step;
    pos.y += v.y * step;

    constexpr float kEps = 1e-4f;
    if (std::abs((v.x ? (target_->x - pos.x) : (target_->y - pos.y))) <= kEps) {
        if (v.x) pos.x = target_->x; else pos.y = target_->y;
        target_.reset();

        if (queued_dir_ != EDirection::NONE && queued_dir_ != dir_) {
            const auto nv = FromDirectionToVector(queued_dir_);
            Coords_t next_cell = pos + Coords_t{params_.tile_size * nv.x, params_.tile_size * nv.y};
            if (grid.AreCoordsWalkable(next_cell)) {
                dir_ = queued_dir_;
            }
        }
    }
}

void GridMover::SetNextDirection(EDirection dir) {
    queued_dir_ = dir;
}

void GridMover::ClearQueue() {
    queued_dir_ = EDirection::NONE;
}

void GridMover::Stop() {
    dir_ = EDirection::NONE;
    target_.reset();
}

EDirection GridMover::Direction() const noexcept {
    return dir_;
}

}
