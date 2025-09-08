#pragma once

#include <SDL2/SDL.h>

namespace Engine2D {

constexpr inline SDL_FRect operator*(const SDL_FRect& rect, float scalar) {
    return {rect.x * scalar, rect.y * scalar, rect.w * scalar, rect.h * scalar};
}

}
