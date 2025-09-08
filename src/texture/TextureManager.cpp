#include "texture/TextureManager.hpp"

TextureManager::TextureManager(SDL_Renderer& renderer) 
    : renderer_(renderer) {}

TextureManager::~TextureManager() {
    ClearAllTextures();
}

SDL_Texture* TextureManager::LoadTexture(const std::string& file_path, const std::string& id) {
    if (textures_.count(id) == 0) {
        SDL_Texture* texture = IMG_LoadTexture(&renderer_, file_path.c_str());
        if (!texture) {
            SDL_Log("Failed to load texture: %s. SDL Error: %s", file_path.c_str(), SDL_GetError());
            return nullptr;
        }
        SDL_Log("[TEXTURE] Loading new texture: %s", file_path.c_str());
        textures_[id] = texture;
    }
    return textures_[id];
}

SDL_Texture* TextureManager::GetTexture(const std::string& id) {
    auto it = textures_.find(id);
    if (it != textures_.end()) {
        return it->second;
    }
    return nullptr;
}

void TextureManager::RemoveTexture(const std::string& file_path) {
    auto it = textures_.find(file_path);
    if (it != textures_.end()) {
        SDL_DestroyTexture(it->second);
        textures_.erase(it);
    }
}

void TextureManager::ClearAllTextures() {
    for (auto& texture_pair : textures_) {
        SDL_DestroyTexture(texture_pair.second);
    }
    textures_.clear();
}