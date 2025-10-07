#include "font/FontManager.hpp"

#include "asset_locator/AssetLocator.hpp"

namespace Engine2D {

FontManager::FontManager(AssetLocator& locator) 
    : locator_(locator) {}

FontManager::~FontManager() {
    ClearFonts();
}

TTF_Font* FontManager::GetFont(const std::string& font_id) {
    auto it = fonts_.find(font_id);
    return (it == fonts_.end()) ? nullptr : it->second.get(); 
}

TTF_Font* FontManager::LoadFont(const std::string& rel_path, int font_size, const std::string& id) {
    const std::string new_id = id.empty()
        ? (rel_path + "#" + std::to_string(font_size))
        : id;

    if (auto* font = GetFont(new_id)) {
        return font;
    }

    const auto full = locator_.Resolve(rel_path);
    SDL_Log("[FONT] Loading: %s", full.string().c_str());

    TTF_Font* raw = TTF_OpenFont(full.string().c_str(), font_size);
    if (!raw) {
        SDL_Log("Failed to load: %s. FONT: %s", full.string().c_str(), TTF_GetError());
        return nullptr;
    }

    auto [it, inserted] = fonts_.try_emplace(id, raw);
    if (!inserted) {
        TTF_CloseFont(raw);
    }

    return it->second.get();
}

void FontManager::RemoveFont(const std::string& id) {
    auto it = fonts_.find(id);
    if (it != fonts_.end()) {
        fonts_.erase(it);
        SDL_Log("[FONT] Removed: %s", id.c_str());
    }
}

void FontManager::ClearFonts() {
    fonts_.clear();
}

}
