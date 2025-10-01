#pragma once

#include "pathfinding/concept/GridGraph.hpp"

#include "utils/Types.hpp"

namespace Engine2D::Pathfinding::Heuristic
{

struct Chebyshev {
    template<Concept::GridGraph G>
    DistanceCost_t operator()(const G& g, NodeId_t a, NodeId_t b) const {
        const auto [ax, ay] = g.IndexToColRow(a);
        const auto [bx, by] = g.IndexToColRow(b);
        return static_cast<DistanceCost_t>(
            std::max(std::abs(ax - bx), std::abs(ay - by)));
    }
};

}
