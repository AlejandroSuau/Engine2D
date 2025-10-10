#pragma once

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "math/Vec2.hpp"

#include <string>

namespace Engine2D {

class Renderer {
public:
    Renderer(SDL_Renderer& renderer);

    void SetRenderingColor(const SDL_Color& color);
    void RenderRect(const SDL_FRect& rect);
    void RenderRectFilled(const SDL_FRect& rect);
    void RenderTexture(
        SDL_Texture* texture,
        const SDL_Rect& src_rect,
        const SDL_FRect& dst_rect,
        double angle = 0);
    void RenderText(
        TTF_Font& font,
        const std::string& text,
        SDL_Color color,
        Vec2<int> coords,
        bool centered = true);
    void RenderLine(Vec2<float> point_from, Vec2<float> point_to);

private:
    SDL_Renderer* renderer_;
};

}
