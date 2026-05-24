#pragma once

#include "IEngineAPI.hpp"
#include "game/IGame.hpp"
#include "renderer/Renderer.hpp"

#include <string>

class Game : public Engine2D::IGame {
public:
    Game(Engine2D::IEngineAPI& engine_api);

    void Start() override;
    void Update(float dt) override;
    void Render(Engine2D::Renderer& renderer) override;
    void HandleEvent(const SDL_Event& event) override;
    std::string GetWindowTitle() override;
    int GetScreenWidth() override;
    int GetScreenHeight() override;

private:
    Engine2D::IEngineAPI& engine_api_;
};