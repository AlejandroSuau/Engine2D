#pragma once

#include "pathfinding/detail/StaticVec.hpp"

#include "utils/Types.hpp"

namespace Engine2D 
{

using Neigh = std::pair<NodeId_t, DistanceCost_t>;

struct CrossProvider {
    template<class Grid>
    auto operator()(const Grid& g, NodeId_t u) const {
        const auto cr = g.IndexToColRow(u);
        const auto neighbours = g.GetNeighboursCross(cr);
        StaticVec<Neigh, 4> out;
        for (auto& opt : neighbours) {
            if (!opt) continue;
            
            out.push_back({opt->index_, opt->cost_});
        }

        return out;
    }
};

}