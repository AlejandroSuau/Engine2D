#pragma once

#include <SDL2/SDL.h>

#include "game/IGame.hpp"

#include "grid/Grid.hpp"
#include "game/pacman/Entity.hpp"

#include <memory>

class Game : public Engine2D::IGame {
public:    
    void Start() override;
    void Update(float dt) override;
    void Render(Engine2D::Renderer& renderer) override;
    void HandleEvent(const SDL_Event& event) override;
    virtual std::string GetWindowTitle() override;
    virtual int GetScreenWidth() override;
    virtual int GetScreenHeight() override;

    const Entity& GetPlayer() const;
    const Engine2D::Grid& GetGrid() const;

    Engine2D::Grid grid_ {
        Vec2<float>{0.f, 0.f},
        20,
        20,
        20
    };

    std::unique_ptr<Entity> ghost_;
    std::unique_ptr<Entity> player_;
};