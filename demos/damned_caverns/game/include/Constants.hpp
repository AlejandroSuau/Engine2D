#pragma once

#include "Vec2.hpp"

#include <vector>
#include <array>

static constexpr int SCREEN_WIDTH = 400;
static constexpr int SCREEN_HEIGHT = 400;

// Seteamos variables para los métodos de Update y Render independientemente de la potencia procesamiento.
static constexpr uint64_t kTargetFPS = 60;
static constexpr uint64_t kFixedTimeStep = 1000 / kTargetFPS;
static constexpr uint64_t kFrameDelay = 1000 / kTargetFPS; // Frame delay en milliseconds

static constexpr float kDemoTimeLimit = 40.0f;
static constexpr float kBPM = 120.0f;
static constexpr float kBeatDuration = 60.f / kBPM;

// Map
/*Vec2<int> startX = {2, 7};
Vec2<int> monsterX = {5, 4};
Vec2<int> exitX = {8, 8};*/

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