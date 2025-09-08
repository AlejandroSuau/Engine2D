#include "Engine2D.hpp"

#include <SDL2/SDL.h>

#include "game/IGame.hpp"
#include "grid/Grid.hpp"

#include <memory>

class Game : public Engine2D::IGame {
public:    
    void Start() {

    }

    void Update(float dt) {

    }

    void Render(SDL_Renderer* renderer) {
        const auto& cells = grid_.Cells();
        const auto cell_dimensions = static_cast<float>(grid_.GetCellDimensions());
        for (const auto& c : cells) {
            SDL_SetRenderDrawColor(renderer, 150, 150, 0, 255);
            SDL_FRect r{c.top_left_.x, c.top_left_.y, cell_dimensions, cell_dimensions};
            SDL_RenderDrawRectF(renderer, &r);
        }
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
};

int main(int argc, char** argv) {
    Engine2D::Engine eng;
    eng.Run(std::make_unique<Game>());

    return 0;
}