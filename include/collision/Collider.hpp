#pragma once

#include <variant>

#include "collision/ColliderType.hpp"
#include "math/Vec2.hpp"

namespace Engine2D {

using ColliderShape = std::variant<Circle, AABB>;

struct Collider {
    std::uint32_t id;
    ColliderShape shape;
    std::uint32_t layer {0x0u};
    std::uint32_t mask {0x0u};

    void SetCenterPosition(Vec2<float> center) noexcept {
        std::visit([&](auto& s) {
            s.center = center;
        }, shape);
    }

    Vec2<float> GetCenterPosition() const noexcept {
        return std::visit([](const auto& s) {
            return s.center;
        }, shape);
    }
};

}
