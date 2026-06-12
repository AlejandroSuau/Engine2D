#pragma once

#include "Vec2.hpp"

#include <algorithm>

#include "Constants.hpp"

enum class EType {
    NORMAL
};

enum class EColor {
    WHITE,
    BLUE,
    GREEN,
    RED,
    PURPLE
};

struct Block {
    EType type_;
    EColor color_;
    Vec2<float> position_;
    Vec2<float> velocity_;
    Vec2<float> acceleration_;

    Vec2<bool> is_moving_;
    Vec2<float> destination_;

    void Update(float dt) {
        if (is_moving_.x) {
            velocity_.x += acceleration_.x * dt;
            position_.x += velocity_.x * dt;
            if (position_.x >= destination_.x) {
                position_.x = destination_.x;
                velocity_.x = kBlockVelocityX;
                is_moving_.x = false;
            }
        }

        if (is_moving_.y) {
            velocity_.y += acceleration_.y * dt;
            position_.y += velocity_.y * dt;
            if (position_.y >= destination_.y) {
                position_.y = destination_.y;
                is_moving_.y = false;
                velocity_.y = kBlockVelocityY;
            }
        }
    }
};
