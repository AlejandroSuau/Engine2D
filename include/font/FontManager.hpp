#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <unordered_map>
#include <string>
#include <memory>

namespace Engine2D {

class AssetLocator;

class FontManager final {
public:
    // Reason of this: MSVC was causing issues using plain TTF_CloseFont on FontPtr.
    struct SDLFontDeleter {
        void operator()(TTF_Font* f) const noexcept { if (f) TTF_CloseFont(f); }
    };
    using FontPtr = std::unique_ptr<TTF_Font, SDLFontDeleter>;

    FontManager(AssetLocator& locator);
    ~FontManager();

    TTF_Font* GetFont(const std::string& id);
    TTF_Font* LoadFont(const std::string& rel_path, int font_size, const std::string& id);
    void RemoveFont(const std::string& id);

private:
    FontManager(const FontManager&) noexcept = delete;
    FontManager(FontManager&&) noexcept = delete;
    FontManager& operator=(const FontManager&) noexcept = delete;
    FontManager& operator=(FontManager&&) noexcept = delete;

    AssetLocator& locator_;
    std::unordered_map<std::string, FontPtr> fonts_;

    void ClearFonts();
};

}
