#pragma once

#include "pathfinding/concept/GridGraph.hpp"

#include "utils/Types.hpp"

namespace Engine2D
{

struct ZeroHeuristic {
    template<GridGraph G>
    DistanceCost_t operator()(const G&, NodeId_t, NodeId_t) const { return 0; }
};

}
