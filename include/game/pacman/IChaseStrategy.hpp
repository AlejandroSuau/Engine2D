#pragma once

#include "utils/Types.hpp"

class Game;

class IChaseStrategy {
public:
    virtual ~IChaseStrategy() = default;
    virtual Engine2D::Coords_t ChasingTargetPosition(const Game& game_context) = 0; 
};

class ChaseStrategyBlinky : public IChaseStrategy {
public:
    Engine2D::Coords_t ChasingTargetPosition(const Game& game_context) override;
};

class ChaseStrategyPinky : public IChaseStrategy {
public:
    Engine2D::Coords_t ChasingTargetPosition(const Game& game_context) override;
};

class ChaseStrategyInky : public IChaseStrategy {
public:
    Engine2D::Coords_t ChasingTargetPosition(const Game& game_context) override;
};

class ChaseStrategyClyde : public IChaseStrategy {
public:
    Engine2D::Coords_t ChasingTargetPosition(const Game& game_context) override;
};