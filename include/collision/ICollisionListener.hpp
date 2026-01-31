#pragma once

#include "collision/Collider.hpp"

namespace Engine2D {

struct CollisionEvent {
    uint32_t id_left;
    uint32_t id_right;
};

class ICollisionListener {
public:
    virtual ~ICollisionListener() = default;
    virtual void OnCollision(const CollisionEvent& e) = 0;
};
    
};
