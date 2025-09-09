#include "grid/GridMovement.hpp"

namespace Engine2D {

EDirection OppositeDirection(EDirection direction) {
    switch (direction) {
        default:
        case EDirection::UP:     return EDirection::DOWN;
        case EDirection::RIGHT:  return EDirection::LEFT;
        case EDirection::DOWN:   return EDirection::UP;
        case EDirection::LEFT:   return EDirection::RIGHT;
    }
}

Coords_t FromDirectionToVector(EDirection direction) {
    switch(direction) {
        case EDirection::UP:     return { 0.f, -1.f};
        case EDirection::RIGHT:  return { 1.f,  0.f};
        case EDirection::DOWN:   return { 0.f,  1.f};
        case EDirection::LEFT:   return {-1.f,  0.f};
        default:                 
        case EDirection::NONE:   return { 0.f,  0.f};
    }
}

bool IsHorizontally(Vec2<float> direction) {
    return direction.x != 0.0f;
}

bool IsVertically(Vec2<float> direction) {
    return direction.y != 0.0f;
}

bool DidReachTarget(float pos, float target, float dir) {
    return (dir > 0.f ? pos >= target : pos <= target);
}

EDirection ResultDirection(Coords_t from, Coords_t to) {
    if (from.x < to.x) return EDirection::RIGHT;
    if (from.x > to.x) return EDirection::LEFT;
    if (from.y < to.y) return EDirection::DOWN;
    if (from.y > to.y) return EDirection::UP;

    return EDirection::NONE;
}

}