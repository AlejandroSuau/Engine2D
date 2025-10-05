#pragma once

#include "utils/Types.hpp"

#include <queue>
#include <tuple>
#include <optional>

namespace Engine2D::Pathfinding::Frontier
{

struct BFSFrontier {
    std::queue<std::tuple<NodeId_t, DistanceCost_t, std::optional<NodeId_t>>> q;

    void push(NodeId_t n, DistanceCost_t g, DistanceCost_t /*h*/, std::optional<NodeId_t> p) {
        q.emplace(n, g, p);
    }
    
    bool empty() const {
        return q.empty();
    }

    auto pop() {
        auto t = q.front();
        q.pop();
        return t;
    }

    void clear() {
        q = {};
    }
};

}
