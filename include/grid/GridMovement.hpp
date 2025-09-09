#pragma once

#include "utils/Types.hpp"

namespace Engine2D {

enum class EDirection {
    NONE,
    UP,
    RIGHT,
    LEFT,
    DOWN
};

EDirection OppositeDirection(EDirection direction);
Coords_t FromDirectionToVector(EDirection direction);
bool IsHorizontally(Vec2<float> direction);
bool IsVertically(Vec2<float> direction);
bool DidReachTarget(float pos, float target, float dir);
EDirection ResultDirection(Coords_t from, Coords_t to);

}
