#pragma once

#include "Vec2.hpp"

#include <cstdint>
#include <array>

static constexpr int SCREEN_WIDTH = 400;
static constexpr int SCREEN_HEIGHT = 400;

// Seteamos variables para los métodos de Update y Render independientemente de la potencia procesamiento.
static constexpr uint64_t kTargetFPS = 60;
static constexpr uint64_t kFixedTimeStep = 1000 / kTargetFPS;
static constexpr uint64_t kFrameDelay = 1000 / kTargetFPS; // Frame delay en milliseconds

// Map
static const float kPlayerDimensions = 40.f;
static const Vec2<int> kPlayerStartColRow {2, 7};
static const Vec2<int> kMonsterStartColRow {5, 4};
static const Vec2<int> kExitColRow {8, 8};

static const std::size_t kMapColumnCount = 10;
static const std::size_t kMapRowCount = 10;
static const std::size_t kCellDimensions = 40;
static const float kCellDimensionsF = static_cast<float>(kCellDimensions);
// 1 = walkable, 0 = wall
static const std::array<std::array<bool, kMapColumnCount>, kMapRowCount> kMapData {{
    {{0,0,0,0,0,0,0,0,0,0}},
    {{0,1,1,1,0,1,1,1,1,0}},
    {{0,1,1,1,0,1,1,1,1,0}},
    {{0,1,0,1,0,1,1,1,1,0}},
    {{0,1,1,1,1,1,1,0,1,0}},
    {{0,1,1,1,1,1,1,0,1,0}},
    {{0,1,1,0,1,1,1,0,1,0}},
    {{0,1,1,0,1,1,1,0,1,0}},
    {{0,1,1,0,1,1,1,0,1,0}},
    {{0,0,0,0,0,0,0,0,0,0}}
}};