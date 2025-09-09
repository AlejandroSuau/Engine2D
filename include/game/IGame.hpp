#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace Engine2D {
    
class Renderer;

class IGame {
public:
    virtual ~IGame() = default;
    
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(Renderer& renderer) = 0;
    virtual void HandleEvent(const SDL_Event& event) = 0;

    virtual std::string GetWindowTitle() = 0;
    virtual int GetScreenWidth() = 0;
    virtual int GetScreenHeight() = 0;
};

}
