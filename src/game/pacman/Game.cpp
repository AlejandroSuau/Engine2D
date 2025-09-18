#include "game/pacman/Game.hpp"

#include <SDL2/SDL.h>

#include "renderer/Renderer.hpp"

void Game::Start() {
    ghost_ = std::make_unique<Ghost>(
        grid_.TopLeftCoordsToCenterCoords(grid_.ColRowToCoords({2, 5})), 20.f);

    player_ = std::make_unique<Player>(
        grid_.TopLeftCoordsToCenterCoords(grid_.ColRowToCoords({10, 5})), 20.f);
}

void Game::Update(float dt) {
    ghost_->Update(dt, grid_);
    player_->Update(dt, grid_);
}

void Game::Render(Engine2D::Renderer& renderer) {
    // Render Grid
    const auto& cells = grid_.Cells();
    const auto cell_dimensions = static_cast<float>(grid_.GetCellDimensions());
    renderer.SetRenderingColor({195, 195, 195, 100});
    for (const auto& c : cells) {
        renderer.RenderRect(
            {c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions}
        );
    }

    // Render Player
    renderer.SetRenderingColor({100, 0, 100, 255});
    renderer.RenderRectFilled(player_->GetRectCentered());

    // Render Ghosts
    renderer.SetRenderingColor({200, 0, 0, 255});
    renderer.RenderRectFilled(ghost_->GetRectCentered());

    auto target_pos = ghost_->mover.FinalTarget();
    if (target_pos) {
        renderer.SetRenderingColor({50, 255, 0, 255});
        renderer.RenderRectFilled({
            target_pos->x - ghost_->dimensions_ / 2.f, 
            target_pos->y - ghost_->dimensions_ / 2.f,
            ghost_->dimensions_,
            ghost_->dimensions_});
    }
}

const Entity& Game::GetPlayer() const {
    return *player_;
}

const Engine2D::Grid& Game::GetGrid() const {
    return grid_;
}

void Game::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONUP) {
        const auto colrow = grid_.CoordsToColRow(
            {static_cast<float>(event.button.x), static_cast<float>(event.button.y)});
        const auto coords = grid_.TopLeftCoordsToCenterCoords(grid_.ColRowToCoords(colrow));
        ghost_->mover.SetFinalTarget(coords);
    }

    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.scancode == SDL_SCANCODE_W) {
            player_->mover.SetNextDirection(Engine2D::EDirection::UP);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_A) {
            player_->mover.SetNextDirection(Engine2D::EDirection::LEFT);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
            player_->mover.SetNextDirection(Engine2D::EDirection::DOWN);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_D) {
            player_->mover.SetNextDirection(Engine2D::EDirection::RIGHT);
        }

        if (event.key.keysym.scancode == SDL_SCANCODE_C) {
            ghost_->mover.SetFinalTarget(ghost_->chase_strategy_->ChasingTargetPosition(*this));
        }
    }
}

std::string Game::GetWindowTitle() { return "Hello Alejandro"; }
int Game::GetScreenWidth() { return 400; }
int Game::GetScreenHeight() { return 400; }