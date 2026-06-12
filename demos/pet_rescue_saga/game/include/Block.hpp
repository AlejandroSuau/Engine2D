#pragma once

enum class EType {
    NORMAL
};

enum class EColor {
    BLUE,
    GREEN,
    RED,
    PURPLE
};

struct Block {
    EType type_;
    EColor color_;
    float x_;
    float y_;
};
