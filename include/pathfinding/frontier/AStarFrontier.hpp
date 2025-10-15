#pragma once

#include "utils/Types.hpp"

#include <queue>
#include <vector>
#include <tuple>
#include <optional>

namespace Engine2D::Pathfinding::Frontier
{

struct AStarFrontier {
    struct Item {
        NodeId_t n;
        DistanceCost_t g;
        DistanceCost_t h;
        std::optional<NodeId_t> p;
        
        DistanceCost_t f() const noexcept {
            constexpr DistanceCost_t INF = std::numeric_limits<DistanceCost_t>::max();
            return (h > INF - g) ? INF : (g + h);
        }
    };
    struct Cmp {
        bool operator()(const Item& a, const Item& b) const noexcept {
            const auto fa = a.f();
            const auto fb = b.f();
            if (fa != fb) return fa > fb;    
            if (a.h != b.h) return a.h > b.h;
            return a.n > b.n;                
        }
    };

    std::priority_queue<Item, std::vector<Item>, Cmp> pq;

    void push(NodeId_t n, DistanceCost_t g, DistanceCost_t h_cost, std::optional<NodeId_t> p) {
        pq.emplace(n, g, h_cost, p);
    }

    bool empty() const {
        return pq.empty();
    }

    auto pop() { 
        const auto it = pq.top();
        pq.pop(); 
        return std::tuple<NodeId_t, DistanceCost_t, std::optional<NodeId_t>>{it.n, it.g, it.p};
    }

    void clear() {
        pq = {};
    }
};

}
