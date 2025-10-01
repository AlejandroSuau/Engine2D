#pragma once

#include "pathfinding/detail/StaticVec.hpp"
#include "grid/Grid.hpp"
#include "utils/Types.hpp"

namespace Engine2D::Pathfinding::NeighbourProvider
{

struct StarProvider {
    template<class Grid>
    auto operator()(const Grid& g, NodeId_t u) const {
        const auto cr = g.IndexToColRow(u);
        const auto neighbours = g.GetNeighboursStar(cr);
        StaticVec<Neighbour, 8> out;
        for (auto& opt : neighbours) {
            if (!opt) continue;
            
            out.push_back({opt->index_, opt->cost_});
        }

        return out;
    }
};

}