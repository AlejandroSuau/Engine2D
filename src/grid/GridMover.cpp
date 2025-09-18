#include "grid/GridMover.hpp"

#include <SDL2/SDL.h>

#include <math.h>

namespace Engine2D {

GridMover::GridMover(Params p) 
    : params_(p) {}

void GridMover::Update(float dt, const Grid& grid, Coords_t& pos) {
    const float speed = params_.tiles_per_sec * params_.tile_size;
    if (speed <= 0.f) return;

    // Set new target
    if (!step_target_) {
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
        step_target_ = candidate;
    }

    // Move
    const auto v = FromDirectionToVector(dir_);
    const float max_step = speed * dt;
    
    float remaining = 0.f;
    if (IsHorizontally(v)) remaining = (step_target_->x - pos.x) * v.x;
    else if (IsVertically(v)) remaining = (step_target_->y - pos.y) * v.y;

    const float step = std::min(max_step, std::max(0.f, remaining));
    pos += Coords_t{v.x * step, v.y * step};

    if (IsAtTarget(pos, *step_target_)) {
        pos = *step_target_;
        step_target_.reset();
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
    step_target_.reset();
}

EDirection GridMover::Direction() const noexcept {
    return dir_;
}

std::optional<Coords_t> GridMover::FinalTarget() const {
    return final_target_;
}

void GridMover::SetFinalTarget(Coords_t pos) {
    final_target_ = pos;
}

}
