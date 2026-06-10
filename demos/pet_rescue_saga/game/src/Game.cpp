#include "Game.hpp"

Game::Game(Engine2D::IEngineAPI& engine_api) 
    : engine_api_(engine_api) {}

void Game::Start() {
    engine_api_.SetCursorDisplay(false);
}

void Game::Update(float dt) {}

void Game::Render(Engine2D::Renderer& renderer) {}
std::string Game::GetWindowTitle() { return "Pathfinder Demo"; }
int Game::GetScreenWidth() { return 800; }
int Game::GetScreenHeight() { return 800; }
