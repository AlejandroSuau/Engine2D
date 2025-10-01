#pragma once

#include "utils/Types.hpp"

#include <optional>
#include <tuple>

namespace Engine2D::Pathfinding::Concept
{

template<class F>
concept Frontier = requires(F f) {
    // (node, g_cost, h_cost, parent)
    { f.push(NodeId_t{}, DistanceCost_t{}, DistanceCost_t{}, std::optional<NodeId_t>{}) };
    { f.empty() } -> std::convertible_to<bool>;
    // (node, g_cost, parent)
    { f.pop() } -> std::same_as<std::tuple<NodeId_t, DistanceCost_t, std::optional<NodeId_t>>>;
    { f.clear() };
};

}