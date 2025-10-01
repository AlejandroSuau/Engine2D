#pragma once

#include "utils/Types.hpp"

namespace Engine2D::Pathfinding::Concept
{

template<class G>
concept GridGraph = requires(const G& g, NodeId_t u) {
    { g.GetCellCount() } -> std::convertible_to<std::size_t>;
    { g.IndexToColRow(u) };
};

}