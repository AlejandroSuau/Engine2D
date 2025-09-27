#pragma once

#include "pathfinding/detail/StaticVec.hpp"

#include "utils/Types.hpp"

namespace Engine2D 
{

struct StarProvider {
    template<class Grid>
    auto operator()(const Grid& g, NodeId_t u) const {
        auto cr = g.IndexToColRow(u);
        auto neighbours = g.GetNeighboursStar(cr);
        StaticVec<Neigh, 8> out;
        for (auto& opt : neighbours) {
            if (!opt) continue;
            
            out.push_back({opt->index_, opt->cost_});
        }

        return out;
    }
};

}