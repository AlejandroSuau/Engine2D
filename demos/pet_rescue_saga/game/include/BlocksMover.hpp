#pragma once

class Grid;

class BlocksMover {
public:
    BlocksMover(Grid& grid);

    void Update(float dt);

private:
    Grid& grid_;
    bool is_moving_top_down_;
    bool is_moving_right_left_;

    void MoveBlocksTopDown(float dt);
    void MoveBlocksRightLeft(float dt);
};