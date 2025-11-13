#pragma once

#include <SDL2/SDL.h>

namespace Engine2D {

class IEngineAPI {
public:
    virtual ~IEngineAPI() = default;

    virtual SDL_Renderer* GetSDLRenderer() noexcept = 0;
    virtual void SetCursorDisplay(bool show) = 0;
};

}
