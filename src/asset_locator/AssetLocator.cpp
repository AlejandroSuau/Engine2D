#include "asset_locator/AssetLocator.hpp"

namespace Engine2D {

AssetLocator::AssetLocator() {
    if (char* p = SDL_GetBasePath()) {
        base_ = p;
        SDL_free(p);
    } else {
        base_ = std::filesystem::current_path();
    }

    paths_.emplace_back(base_ / "assets");
}

std::filesystem::path AssetLocator::Resolve(std::string_view rel) const {
    for (const auto& root : paths_) {
        auto p = root / rel;
        std::error_code ec{};
        if (std::filesystem::exists(p, ec)) return p;
    }

    return paths_.front() / rel;
}

void AssetLocator::LogPaths() const {
    SDL_Log("Base: %s", base_.string().c_str());
    for (const auto& r : paths_) {
        SDL_Log("Asset root: %s", r.string().c_str());
    }
}

}
