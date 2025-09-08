#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace Engine2D {

class IGame {
public:
    virtual ~IGame() = default;
    
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
    virtual void HandleEvents() = 0;

    virtual std::string GetWindowTitle() = 0;
    virtual int GetScreenWidth() = 0;
    virtual int GetScreenHeight() = 0;
};

}
