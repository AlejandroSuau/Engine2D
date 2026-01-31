#pragma once

#include "collision/ColliderType.hpp"
#include "collision/Collider.hpp"

#include <cmath>

namespace Engine2D {

inline bool CanCollide(const Collider& a, const Collider& b) noexcept {
    const bool a_hits_b = (a.mask & b.layer) != 0u;
    const bool b_hits_a = (b.mask & a.layer) != 0u;
    return a_hits_b && b_hits_a;
}

inline bool Intersects(const Circle& a, const Circle& b) noexcept {
    const auto diff = a.center - b.center;
    const float dist2 = diff.LengthSquared();
    const float r = a.radius + b.radius;
    return (dist2 <= r * r);
}

inline bool Intersects(const AABB& a, const AABB& b) noexcept {
    const bool no_overlap_x =
        (a.center.x + a.half_extents.x) < (b.center.x - b.half_extents.x) ||
        (b.center.x + b.half_extents.x) < (a.center.x - a.half_extents.x);

    const bool no_overlap_y =
        (a.center.y + a.half_extents.y) < (b.center.y - b.half_extents.y) ||
        (b.center.y + b.half_extents.y) < (a.center.y - a.half_extents.y);

    return !(no_overlap_x || no_overlap_y);
}

inline bool Intersects(const Circle& circle, const AABB& box) noexcept {
    const float left   = box.center.x - box.half_extents.x;
    const float right  = box.center.x + box.half_extents.x;
    const float top    = box.center.y - box.half_extents.y;
    const float bottom = box.center.y + box.half_extents.y;

    const float closest_x = std::max(left,  std::min(circle.center.x, right));
    const float closest_y = std::max(top,   std::min(circle.center.y, bottom));

    const Vec2 diff{circle.center.x - closest_x, circle.center.y - closest_y};
    const float dist2 = diff.LengthSquared();
    return (dist2 <= (circle.radius * circle.radius));
}

inline bool Intersects(const AABB& box, const Circle& circle) noexcept {
    return Intersects(circle, box);
}

inline bool Intersects(const ColliderShape& a, const ColliderShape& b) noexcept {
    return std::visit([](const auto& lhs, const auto& rhs) noexcept {
        return Intersects(lhs, rhs);
    }, a, b);
}

inline bool Intersects(const Collider& a, const Collider& b) noexcept {
    return Intersects(a.shape, b.shape);
}

}
