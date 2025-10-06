#pragma once

#include "texture/AssetLocator.hpp"

#include <unordered_map>
#include <string>
#include <memory>

#include <SDL_image.h>

namespace Engine2D {

class TextureManager {
public:
    // Reason of this: MSVC was causing issues using plain SDL_DestroyTexture on TexturePtr.
    struct SDLTextureDeleter {
        void operator()(SDL_Texture* p) const noexcept { if (p) SDL_DestroyTexture(p); }
    };
    using TexturePtr = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

    TextureManager(SDL_Renderer& renderer);
    ~TextureManager();

    SDL_Texture* LoadTexture(const std::string& rel_path, const std::string& id);
    SDL_Texture* Get(const std::string& id);
    void RemoveById(const std::string& id);

private:
    TextureManager(const TextureManager&) noexcept = delete;
    TextureManager(TextureManager&&) noexcept = delete;
    TextureManager& operator=(const TextureManager&) noexcept = delete;
    TextureManager& operator=(TextureManager&&) noexcept = delete;

    SDL_Renderer& renderer_;
    AssetLocator locator_;
    std::unordered_map<std::string, TexturePtr> textures_;

    void ClearAll();
};

}
