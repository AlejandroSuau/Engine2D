#include "texture/TextureManager.hpp"

namespace Engine2D {

TextureManager::TextureManager(SDL_Renderer& renderer) 
    : renderer_(renderer) {
    locator_.LogPaths();
}

TextureManager::~TextureManager() {
    ClearAll();
}

SDL_Texture* TextureManager::LoadTexture(const std::string& rel_path, const std::string& id) {
    if (auto* texture = Get(id)) {
        return texture;
    }

    const auto full = locator_.Resolve(rel_path);
    SDL_Log("[TEXTURE] Loading: %s", full.string().c_str());

    SDL_Texture* raw = IMG_LoadTexture(&renderer_, full.string().c_str());
    if (!raw) {
        SDL_Log("Failed to load: %s. IMG: %s", full.string().c_str(), IMG_GetError());
        return nullptr;
    }

    auto [it, inserted] = textures_.try_emplace(id, raw);
    if (!inserted) {
        SDL_DestroyTexture(raw);
    }

    return it->second.get();
}

SDL_Texture* TextureManager::Get(const std::string& id) {
    auto it = textures_.find(id);
    if (it != textures_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void TextureManager::RemoveById(const std::string& id) {
    auto it = textures_.find(id);
    if (it != textures_.end()) {
        textures_.erase(it);
    }
}

void TextureManager::ClearAll() {
    textures_.clear();
}

}
