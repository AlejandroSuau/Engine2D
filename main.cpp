#include "Engine2D.hpp"

#include <SDL2/SDL.h>

#include "game/IGame.hpp"

#include "renderer/Renderer.hpp"

#include "grid/Grid.hpp"
#include "grid/GridMover.hpp"
#include "grid/GridMoverUtils.hpp"

#include "utils/Types.hpp"

#include <memory>
#include <optional>

struct Entity {
public:
    Engine2D::Coords_t pos_ {}; 
    float dimensions_ {0.f};
    Engine2D::EDirection direction_ {Engine2D::EDirection::RIGHT};

    Engine2D::GridMover mover{{.tile_size = dimensions_, .tiles_per_sec = 10.f}};

    std::optional<Engine2D::EDirection> next_direction_{Engine2D::EDirection::RIGHT}; // ONLY FOR PLAYER
    std::optional<Engine2D::Coords_t> next_step_;
    std::optional<Engine2D::Coords_t> target_pos_;

    Entity(Engine2D::Coords_t pos, float dim) 
        : pos_(pos), dimensions_(dim) {}

    virtual ~Entity() = default;
    virtual void Update(float dt, const Engine2D::Grid& grid) = 0; // GameContext instead of Grid

    SDL_FRect GetRect() const { return {pos_.x, pos_.y, dimensions_, dimensions_}; }
    SDL_FRect GetRectCentered() const { return {pos_.x - dimensions_ / 2.f, pos_.y - dimensions_ / 2.f, dimensions_, dimensions_}; }

};

struct Player : public Entity {

    Player(Engine2D::Coords_t pos, float dimensions) 
        : Entity(pos, dimensions) {}

    void Update(float dt, const Engine2D::Grid& grid) override {
        mover.Update(dt, grid, pos_);
    }
};

// 1. player_pos
// 2. player_pos + 4 tiles dir (if up 4 left aswell)
// 3. from blinky to player_pos + 2 tiles dir (if up 2 left aswell) - then double it
// 4. if 8 >= tiles distance go to corner else player_pos

struct Ghost : public Entity {
    
    Ghost(Engine2D::Coords_t pos, float dimensions) 
        : Entity(pos, dimensions) {}

    void Update(float dt, const Engine2D::Grid& grid) override {
        if (target_pos_) {
            mover.SetNextDirection(Engine2D::ResultDirection(pos_, *target_pos_));
        }
        
        mover.Update(dt, grid, pos_);
    }
};

class Game : public Engine2D::IGame {
public:    
    void Start() {
        ghost_ = std::make_unique<Ghost>(
            grid_.TopLeftCoordsToCenterCoords(grid_.ColRowToCoords({2, 5})), 20.f);

        player_ = std::make_unique<Player>(
            grid_.TopLeftCoordsToCenterCoords(grid_.ColRowToCoords({10, 5})), 20.f);
    }

    void Update(float dt) {
        ghost_->Update(dt, grid_);
        player_->Update(dt, grid_);
    }

    void Render(Engine2D::Renderer& renderer) {
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

        if (ghost_->target_pos_) {
            renderer.SetRenderingColor({50, 255, 0, 255});
            renderer.RenderRectFilled({
                ghost_->target_pos_->x - ghost_->dimensions_ / 2.f, 
                ghost_->target_pos_->y - ghost_->dimensions_ / 2.f,
                ghost_->dimensions_,
                ghost_->dimensions_});
        }
    }

    void HandleEvent(const SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONUP) {
            const auto colrow = grid_.CoordsToColRow(
                {static_cast<float>(event.button.x), static_cast<float>(event.button.y)});
            const auto coords = grid_.TopLeftCoordsToCenterCoords(grid_.ColRowToCoords(colrow));
            ghost_->target_pos_ = coords;
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
        }
    }

    virtual std::string GetWindowTitle() { return "Hello Alejandro"; }
    virtual int GetScreenWidth() { return 400; }
    virtual int GetScreenHeight() { return 400; }

    Engine2D::Grid grid_ {
        Vec2<float>{0.f, 0.f},
        20,
        20,
        20
    };

    std::unique_ptr<Entity> ghost_;
    std::unique_ptr<Entity> player_;
};

int main(int argc, char** argv) {
    Engine2D::Engine eng;
    eng.Run(std::make_unique<Game>());

    return 0;
}