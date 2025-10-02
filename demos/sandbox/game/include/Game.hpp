#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "Engine2D.hpp"
#include "game/IGame.hpp"
#include "renderer/Renderer.hpp"
#include "grid/Grid.hpp"
#include "texture/TextureManager.hpp"

class Game : public Engine2D::IGame {
public:
    Game(Engine2D::Engine& engine);

    void Start() override;
    void Update(float dt) override;
    void Render(Engine2D::Renderer& renderer) override;
    void HandleEvent(const SDL_Event& event) override;
    std::string GetWindowTitle() override;
    int GetScreenWidth() override;
    int GetScreenHeight() override;

    const Engine2D::Grid& GetGrid() const;

private:
    Engine2D::Engine& engine_;
    Engine2D::Grid grid_;
    TextureManager texture_manager_;
    SDL_Texture* loadbar_texture_;
};