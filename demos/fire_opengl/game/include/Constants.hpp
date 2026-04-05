#pragma once

#include <vector>

using framebuffer_t = std::vector<uint32_t>;

static constexpr int SCREEN_WIDTH = 800;
static constexpr int SCREEN_HEIGHT = 600;

// Seteamos variables para los métodos de Update y Render independientemente de la potencia procesamiento.
static constexpr uint64_t kTargetFPS = 60;
static constexpr uint64_t kFixedTimeStep = 1000 / kTargetFPS;
static constexpr uint64_t kFrameDelay = 1000 / kTargetFPS; // Frame delay en milliseconds

static constexpr float kDemoTimeLimit = 40.0f;
static constexpr float kBPM = 120.0f;
static constexpr float kBeatDuration = 60.f / kBPM;