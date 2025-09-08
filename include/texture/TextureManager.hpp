#pragma once

#include <unordered_map>
#include <string>

#include <SDL_image.h>

class TextureManager {
public:
    TextureManager(SDL_Renderer& renderer);
    ~TextureManager();

    SDL_Texture* LoadTexture(const std::string& file_path, const std::string& id);
    SDL_Texture* GetTexture(const std::string& id);
    void RemoveTexture(const std::string& id);

private:
    TextureManager(const TextureManager&) noexcept = delete;
    TextureManager(TextureManager&&) noexcept = delete;
    TextureManager& operator=(const TextureManager&) noexcept = delete;
    TextureManager& operator=(TextureManager&&) noexcept = delete;

    SDL_Renderer& renderer_;
    std::unordered_map<std::string, SDL_Texture*> textures_;

    void ClearAllTextures();
};