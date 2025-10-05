#pragma once

#include "pathfinding/concept/GridGraph.hpp"

#include "utils/Types.hpp"

namespace Engine2D::Pathfinding::Heuristic
{

struct Manhattan {
    template<Concept::GridGraph G>
    DistanceCost_t operator()(const G& g, NodeId_t a, NodeId_t b) const {
        const auto a_colrow = g.IndexToColRow(a);
        const auto b_colrow = g.IndexToColRow(b);
        const auto dx = std::abs(a_colrow.x - b_colrow.x);
        const auto dy = std::abs(a_colrow.y - b_colrow.y);
        return static_cast<DistanceCost_t>(dx + dy);
    }
};

}
