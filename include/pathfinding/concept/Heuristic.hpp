#pragma once

#include "utils/Types.hpp"

namespace Engine2D
{

template<class H, class G>
concept Heuristic = requires(H h, const G& g, NodeId_t a, NodeId_t b) {
    { h(g, a, b) } -> std::convertible_to<DistanceCost_t>;
};

}