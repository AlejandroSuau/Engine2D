#pragma once

#include "Vec2.hpp"

#include <cstdint>

using ColRow_t = Vec2<int>;
using Coords_t = Vec2<float>;

static constexpr int SCREEN_WIDTH = 400;
static constexpr int SCREEN_HEIGHT = 400;

// Seteamos variables para los métodos de Update y Render independientemente de la potencia procesamiento.
static constexpr uint64_t kTargetFPS = 60;
static constexpr uint64_t kFixedTimeStep = 1000 / kTargetFPS;
static constexpr uint64_t kFrameDelay = 1000 / kTargetFPS; // Frame delay en milliseconds

// Grid
static const std::size_t kGridColumnCount = 10;
static const std::size_t kGridRowCount = 10;
static const std::size_t kCellDimensions = 40;
static const float kCellDimensionsF = static_cast<float>(kCellDimensions);

// Block
static constexpr float kBlockVelocityX = 100.f;
static constexpr float kBlockVelocityY = 10.f;
static constexpr float kBlockAccelerationX = 100.f;
static constexpr float kBlockAccelerationY = 300.f;
