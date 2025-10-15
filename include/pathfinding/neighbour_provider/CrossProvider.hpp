#pragma once

#include "pathfinding/detail/StaticVec.hpp"
#include "pathfinding/Pathfinder.hpp"
#include "grid/Grid.hpp"
#include "utils/Types.hpp"

namespace Engine2D::Pathfinding::NeighbourProvider
{

struct CrossProvider {
    template<class Grid>
    auto operator()(const Grid& g, NodeId_t u) const {
        const auto cr = g.IndexToColRow(u);
        const auto neighbours = g.GetNeighboursCross(cr);
        StaticVec<Neighbour, 4> out;
        for (auto& opt : neighbours) {
            if (!opt) continue;
            
            out.push_back({opt->index_, kBaseCostOrthogonal * opt->cost_});
        }

        return out;
    }
};

}