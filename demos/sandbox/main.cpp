#include <SDL2/SDL.h>

#include "Engine2D.hpp"
#include "game/IGame.hpp"
#include "renderer/Renderer.hpp"
#include "grid/Grid.hpp"

struct Game : public Engine2D::IGame {
    void Start() override;
    void Update(float dt) override;
    void Render(Engine2D::Renderer& renderer) override;
    void HandleEvent(const SDL_Event& event) override;
    std::string GetWindowTitle() override;
    int GetScreenWidth() override;
    int GetScreenHeight() override;

    const Engine2D::Grid& GetGrid() const;

    Engine2D::Grid grid_ {
        {0.f, 0.f},
        15,
        15,
        20
    };
};

void Game::Start() {
}

void Game::Update(float dt) {
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
}

const Engine2D::Grid& Game::GetGrid() const {
    return grid_;
}

void Game::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONUP) {
        const auto colrow = grid_.CoordsToColRow(
            {static_cast<float>(event.button.x), static_cast<float>(event.button.y)});
        const auto coords = grid_.TopLeftCoordsToCenterCoords(grid_.ColRowToCoords(colrow));
    }

    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.scancode == SDL_SCANCODE_W) {

        } else if (event.key.keysym.scancode == SDL_SCANCODE_A) {

        } else if (event.key.keysym.scancode == SDL_SCANCODE_S) {

        } else if (event.key.keysym.scancode == SDL_SCANCODE_D) {

        }

        if (event.key.keysym.scancode == SDL_SCANCODE_C) {

        }
    }
}

std::string Game::GetWindowTitle() { return "Hello Alejandro"; }
int Game::GetScreenWidth() { return 400; }
int Game::GetScreenHeight() { return 400; }

int main(int argc, char** argv) {
    Engine2D::Engine eng;
    eng.Run(std::make_unique<Game>());

    return 0;
}
