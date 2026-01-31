#pragma once

#include "math/Vec2.hpp"

namespace Engine2D {

struct AABB {
    Vec2<float> center{};
    Vec2<float> half_extents{};
};

struct Circle {
    Vec2<float> center{};
    float radius;
};

enum class ColliderShapeType : std::uint8_t {
    AABB,
    Circle
};

}
