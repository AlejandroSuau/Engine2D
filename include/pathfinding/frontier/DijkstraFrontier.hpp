#pragma once

#include "utils/Types.hpp"

#include <queue>
#include <optional>
#include <vector>
#include <tuple>

namespace Engine2D::Pathfinding::Frontier
{

struct DijkstraFrontier {
    using Item = std::tuple<NodeId_t, DistanceCost_t, std::optional<NodeId_t>>; // (n, g, p)
    struct Cmp { bool operator()(const Item& a, const Item& b) const {
        return std::get<1>(a) > std::get<1>(b); // min-heap
    }};

    std::priority_queue<Item, std::vector<Item>, Cmp> pq;

    void push(NodeId_t n, DistanceCost_t g, DistanceCost_t /*h*/, std::optional<NodeId_t> p) {
        pq.emplace(n, g, p);
    }
    
    bool empty() const {
        return pq.empty();
    }

    auto pop() {
        auto t = pq.top();
        pq.pop();
        return t;
    }

    void clear() {
        pq = {};
    }
};

}

