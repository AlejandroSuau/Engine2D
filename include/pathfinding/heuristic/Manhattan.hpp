#pragma once

#include "pathfinding/concept/GridGraph.hpp"

#include "utils/Types.hpp"

namespace Engine2D
{

struct Manhattan {
    template<GridGraph G>
    DistanceCost_t operator()(const G& g, NodeId_t a, NodeId_t b) const {
        auto [ax, ay] = g.IndexToColRow(a);
        auto [bx, by] = g.IndexToColRow(b);
        return static_cast<DistanceCost_t>(std::abs(ax - bx) + std::abs(ay - by));
    }
};

}
