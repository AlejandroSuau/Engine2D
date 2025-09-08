#include "font/FontManager.hpp"

FontManager::~FontManager() {
    ClearFonts();
}

TTF_Font* FontManager::GetFont(const std::string& font_id) {
    auto it = fonts_.find(font_id);
    return (it == fonts_.end()) ? nullptr : it->second; 
}

TTF_Font* FontManager::LoadFont(const std::string& file_path, int font_size, const std::string& custom_id) {
    std::string font_id = custom_id.empty() ? file_path : custom_id;
    if (fonts_.count(font_id) == 0) {
        TTF_Font* font = TTF_OpenFont(file_path.c_str(), font_size);
        if (!font) {
            SDL_Log("Failed to load font: %s", file_path.c_str());
            return nullptr;
        }

        SDL_Log("[FONT] Font loaded successfully {path='%s', id='%s'}", file_path.c_str(), font_id.c_str());
        fonts_[font_id] = font;
    }

    return fonts_[font_id];
}

void FontManager::RemoveFont(const std::string& font_id) {
    auto it = fonts_.find(font_id.c_str());
    if (it == fonts_.end()) {
        SDL_Log("[FONT WARNING ]Trying to remove unexpected font: %s", font_id.c_str());
    } else {
        TTF_CloseFont(it->second);
        fonts_.erase(it);
        SDL_Log("[FONT] Removed font: %s", font_id.c_str());
    }
}

void FontManager::ClearFonts() {
    for (const auto& [font_id, font] : fonts_) {
        TTF_CloseFont(font);
    }
    fonts_.clear();
}