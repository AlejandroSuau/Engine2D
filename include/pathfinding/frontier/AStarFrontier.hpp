#pragma once

#include "utils/Types.hpp"

#include <queue>
#include <vector>
#include <tuple>
#include <optional>

namespace Engine2D::Pathfinding::Frontier
{

struct AStarFrontier {
    using Item = std::tuple<NodeId_t, DistanceCost_t, DistanceCost_t, std::optional<NodeId_t>>; // (n, g, f, p)
    struct Cmp { bool operator()(const Item& a, const Item& b) const {
        return std::get<2>(a) > std::get<2>(b); // min-heap for f
    }};

    std::priority_queue<Item, std::vector<Item>, Cmp> pq;

    void push(NodeId_t n, DistanceCost_t g, DistanceCost_t h_cost, std::optional<NodeId_t> p) {
        pq.emplace(n, g, g + h_cost, p);
    }

    bool empty() const {
        return pq.empty();
    }

    auto pop() { 
        auto [n, g, /*f*/_, p] = pq.top();
        pq.pop(); 
        return std::tuple<NodeId_t, DistanceCost_t, std::optional<NodeId_t>>{n, g, p};
    }

    void clear() {
        pq = {};
    }
};

}
