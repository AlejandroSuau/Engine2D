#pragma once

#include "pathfinding/detail/StaticVec.hpp"
#include "pathfinding/Pathfinder.hpp"
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
        std::size_t i = 0;
        for (auto& opt : neighbours) {
            if (opt) {
                auto cost = opt->cost_;
                // First 4 are diagonals.
                cost *= (i < 4) ? kBaseCostDiagonal : kBaseCostOrthogonal;
                out.push_back({opt->index_, cost});
            }
            ++i;
        }

        return out;
    }
};

}