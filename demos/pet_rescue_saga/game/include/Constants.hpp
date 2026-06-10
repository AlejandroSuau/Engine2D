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

// Map
static const std::size_t kMapColumnCount = 10;
static const std::size_t kMapRowCount = 10;
static const std::size_t kCellDimensions = 40;
static const float kCellDimensionsF = static_cast<float>(kCellDimensions);