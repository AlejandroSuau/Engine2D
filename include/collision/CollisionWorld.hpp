#pragma once

#include "collision/Collider.hpp"
#include "collision/ICollisionListener.hpp"
#include "collision/CollisionMath.hpp"

#include <vector>
#include <span>

// This is a O(n^2) system in worst case. May fix it later.

namespace Engine2D {

class CollisionWorld {
public:
    using ColliderID = std::uint32_t;

    CollisionWorld() = default;

    ColliderID AddCollider(Collider&& c) {
        colliders_.push_back(std::move(c));
        ColliderID new_id = static_cast<ColliderID>(colliders_.size() - 1);
        colliders_[new_id].id = new_id;
        return new_id;
    }

    void ClearColliders() {
        colliders_.clear();
    }

    // C++20 span is not type dependant if tomorrow we decide to use i.e. an array.
    // similar a const std::vector<Collider>&.
    std::span<const Collider> Colliders() const noexcept {
        return colliders_;
    }

    Collider& GetCollider(ColliderID id) { return colliders_[id]; }
    const Collider& GetCollider(ColliderID id) const { return colliders_[id]; }

    // Listener
    void AddListener(ICollisionListener* listener) {
        listeners_.push_back(listener);
    }

    void RemoveListener(ICollisionListener* listener) {
        listeners_.erase(
            std::remove(listeners_.begin(), listeners_.end(), listener),
            listeners_.end());
    }

    void CheckCollisionsAndDispatch() {
        const std::size_t n = colliders_.size();
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = i + 1; j < n; ++j) {
                const auto& a = colliders_[i];
                const auto& b = colliders_[j];
                if (CanCollide(a, b) && Intersects(a, b)) {
                    Notify(CollisionEvent{a.id, b.id});
                }
            }
        }
    }

private:
    std::vector<Collider> colliders_;
    std::vector<ICollisionListener*> listeners_;

    void Notify(const CollisionEvent& event) {
        for (auto* listener : listeners_) {
            listener->OnCollision(event);
        }
    }
};

}
