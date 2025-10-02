#include "Game.hpp"

Game::Game(Engine2D::Engine& engine) 
    : engine_(engine)
    , grid_({0.f, 0.f}, 15, 15, 20)
    , texture_manager_(*engine_.SDLRenderer())
    , loadbar_texture_(nullptr) {}

void Game::Start() {
    loadbar_texture_ = texture_manager_.LoadTexture("assets/loadbar.png", "loadbar-1");
}

void Game::Update(float dt) {
}

void Game::Render(Engine2D::Renderer& renderer) {
    // Render Texture
    renderer.RenderTexture(loadbar_texture_, {0, 0, 1132, 71}, {0.f, 0.f, 100.f, 20.f});

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