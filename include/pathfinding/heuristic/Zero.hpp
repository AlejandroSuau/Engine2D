#pragma once

#include "pathfinding/concept/GridGraph.hpp"

#include "utils/Types.hpp"

namespace Engine2D::Pathfinding::Heuristic
{

struct ZeroHeuristic {
    template<Concept::GridGraph G>
    DistanceCost_t operator()(const G&, NodeId_t, NodeId_t) const { return 0; }
};

}
