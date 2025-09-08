#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <unordered_map>
#include <string>

class FontManager final {
public:
    FontManager() = default;
    ~FontManager();

    TTF_Font* GetFont(const std::string& id);
    TTF_Font* LoadFont(const std::string& file_path, int font_size, const std::string& custom_id = "");
    void RemoveFont(const std::string& font_id);

private:
    FontManager(const FontManager&) noexcept = delete;
    FontManager(FontManager&&) noexcept = delete;
    FontManager& operator=(const FontManager&) noexcept = delete;
    FontManager& operator=(FontManager&&) noexcept = delete;

    std::unordered_map<std::string, TTF_Font*> fonts_;

    void ClearFonts();
};