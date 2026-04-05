#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "IEngineAPI.hpp"
#include "game/IGame.hpp"
#include "renderer/Renderer.hpp"

#include "timer/Timer.hpp"

#include <string>
#include <array>

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