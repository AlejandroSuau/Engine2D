#pragma once

#include <SDL2/SDL.h>

#include <filesystem>
#include <vector>
#include <string_view>

namespace Engine2D {

class AssetLocator {
public:
    AssetLocator();

    std::filesystem::path Resolve(std::string_view rel) const;
    void LogPaths() const;

private:
    std::filesystem::path base_;
    std::vector<std::filesystem::path> paths_;
};

}
