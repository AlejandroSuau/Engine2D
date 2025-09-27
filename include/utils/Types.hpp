#include "math/Vec2.hpp"

#include <tuple>

namespace Engine2D {

using ColRow_t = Vec2<int>;
using Coords_t = Vec2<float>;

using NodeId_t = std::size_t;
using DistanceCost_t = int;

using Neighbour = std::pair<NodeId_t, DistanceCost_t>;

}