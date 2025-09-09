#include "Engine2D.hpp"

#include <SDL2/SDL.h>

#include "game/IGame.hpp"

#include "renderer/Renderer.hpp"

#include "grid/Grid.hpp"
#include "grid/GridMovement.hpp"

#include "utils/Types.hpp"

#include <memory>

    /*
    
    const float kTilesPerSec = 3.f;

    Vec2<float> pos_center {};
    Vec2<float> dim {};
    EDirection dir {EDirection::RIGHT};

    std::optional<EDirection> next_direction {std::nullopt};
    std::optional<Vec2<float>> next_step_center {std::nullopt};
    std::optional<Vec2<float>> target_pos_center {std::nullopt};   
    
    MoveLogic move_logic;

    Ent(Vec2<float> p, float d) : pos_center(p), dim{d, d} {}

    void Update(float dt, const GameMap& game_map) {
        Vec2<float> dv = FromDirToVec(dir);
        
        if (!next_step_center) {
            if (next_direction) { // There should be always "next direction"
                // Set next step center
                dir = *next_direction;
                dv = FromDirToVec(dir);
                next_step_center = {
                    pos_center.x + dim.x * dv.x,
                    pos_center.y + dim.y * dv.y};
                next_direction.reset();
            } else {
                return;
            }
        }

        // Move 
        const float speed = kTilesPerSec * dim.x;
        const float step = speed * dt;

        if (IsMovingHorizontally(dv)) {
            pos_center.x += dv.x * step;
            if (DidReachTarget(dv.x, pos_center.x, next_step_center->x)) {
                pos_center.x = next_step_center->x;
                next_step_center.reset();
            }
        } else if (IsMovingVertically(dv)) {
            pos_center.y += dv.y * step;
            if (DidReachTarget(dv.y, pos_center.y, next_step_center->y)) {
                pos_center.y = next_step_center->y;
                next_step_center.reset();
            }
        }
    }*/

struct Entity {
public:
    Engine2D::Coords_t pos_ {};
    float dimensions_ {0.f};
    Engine2D::EDirection direction_ {Engine2D::EDirection::NONE};
    
    std::optional<Engine2D::EDirection> buffered_direction_;
    std::optional<Engine2D::Coords_t> next_step_;
    std::optional<Engine2D::Coords_t> target_pos_center_;

    Entity(Engine2D::Coords_t pos, float dim) 
        : pos_(pos), dimensions_(dim) {}

    virtual ~Entity() = default;
    virtual void Update(float dt, const Engine2D::Grid& grid) = 0; // GameContext instead of Grid

    SDL_FRect GetRect() const { return {pos_.x, pos_.y, dimensions_, dimensions_}; }

};

struct Player : public Entity {
    void Update(float dt, const Engine2D::Grid& grid) override {

    }
};

struct Ghost : public Entity {
    
    Ghost(Engine2D::Coords_t pos, float dimensions) 
        : Entity(pos, dimensions) {}

    void Update(float dt, const Engine2D::Grid& grid) override {
        auto direction_vector = Engine2D::FromDirectionToVector(direction_);
        // Set next step if there's none
        if (!next_step_) {
            if (buffered_direction_) {
                // Update direction
                direction_ = *buffered_direction_;
                direction_vector = Engine2D::FromDirectionToVector(direction_);
                
                // Set next step
                next_step_ = pos_ + Vec2{dimensions_ * direction_vector.x, dimensions_ * direction_vector.y};

                // Reset buffer
                buffered_direction_.reset();
            }
        }

        // Move
        const float kTilesPerSec = 3.f;
        const float speed = kTilesPerSec * dimensions_;
        const float step = speed * dt;
        pos_ += Engine2D::Coords_t{direction_vector.x * step, direction_vector.y * step};

        if (Engine2D::IsHorizontally(direction_vector) &&
            Engine2D::DidReachTarget(pos_.x, next_step_->x, direction_vector.x)) {
            pos_.x = next_step_->x;
            next_step_.reset();
        } else if (Engine2D::IsVertically(direction_vector) &&
                   Engine2D::DidReachTarget(pos_.y, next_step_->y, direction_vector.y)) {
            pos_.y = next_step_->y;
            next_step_.reset();
        }
    }
};

class Game : public Engine2D::IGame {
public:    
    void Start() {
        ghost_ = std::make_unique<Ghost>(grid_.ColRowToCoords({2, 5}), 20.f);
    }

    void Update(float dt) {
        ghost_->Update(dt, grid_);
    }

    void Render(Engine2D::Renderer& renderer) {
        // Render Grid
        const auto& cells = grid_.Cells();
        const auto cell_dimensions = static_cast<float>(grid_.GetCellDimensions());
        renderer.SetRenderingColor({150, 150, 0, 255});
        for (const auto& c : cells) {
            renderer.RenderRect(
                {c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions}
            );
        }

        // Render Player

        // Render Ghosts
        renderer.SetRenderingColor({200, 0, 0, 255});
        renderer.RenderRectFilled(
            {ghost_->pos_.x, ghost_->pos_.x, ghost_->dimensions_, ghost_->dimensions_}
        );
    }

    void HandleEvents() {

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
};

int main(int argc, char** argv) {
    Engine2D::Engine eng;
    eng.Run(std::make_unique<Game>());

    return 0;
}