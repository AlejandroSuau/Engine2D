#include "BlocksMover.hpp"

#include "Grid.hpp"

BlocksMover::BlocksMover(Grid& grid)
    : grid_(grid)
    , is_moving_top_down_(false)
    , is_moving_right_left_(false) {}

void BlocksMover::Update(float dt) {
    MoveBlocksTopDown(dt);
    if (!is_moving_top_down_) {
        MoveBlocksRightLeft(dt);
    }
}

void BlocksMover::MoveBlocksTopDown(float dt) {

}

void BlocksMover::MoveBlocksRightLeft(float dt) {

}


