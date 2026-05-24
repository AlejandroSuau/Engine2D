#include "DamnedCaverns.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

DamnedCaverns::DamnedCaverns(int window_width, int window_height)
    : sdl_initializer_(std::make_unique<SDLInitializer>())
    , sdl_initializer_mixer_(std::make_unique<SDLMixerInitializer>())
    , window_width_(window_width)
    , window_height_(window_height)
    , window_(nullptr, SDL_DestroyWindow)
    , renderer_(nullptr, SDL_DestroyRenderer)
    , is_running_(false)
    , grid_({0, 0}, kMapColumnCount, kMapRowCount, kCellDimensions)
    , player_{
        .direction_ = {-1, 0},
        .position_ = grid_.ColRowToCoords(kPlayerStartColRow),
        .speed_ = 100.0f
    }
{
    std::cout << "Initializing Damned Caverns..." << std::endl;
    CreateWindow();
    SetMapData();
}

DamnedCaverns::~DamnedCaverns() {}

void DamnedCaverns::CreateWindow() {
    window_.reset(SDL_CreateWindow(
        "Damned Caverns - Alejandro Suau Ruiz",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width_,
        window_height_,
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI
    ));
    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED));

    if (!window_ || !renderer_) {
        throw std::runtime_error(
            std::string("Error creating the DemoScene") + SDL_GetError());
    }
}

void DamnedCaverns::SetMapData() {
    std::cout << "Hello world! Setting map data..." << std::endl;
    auto& cells = grid_.Cells();
    for (auto& cell : cells) {
        const auto& colrow = cell.colrow_;
        const auto col = static_cast<std::size_t>(colrow.x);
        const auto row = static_cast<std::size_t>(colrow.y);
        std::cout << "Setting cell at col: " << col << ", row: " << row << " as walkable: " << kMapData[row][col] << std::endl; 
        cell.is_walkable_ = kMapData[row][col];
    }
}

void DamnedCaverns::Run() {
    Init();
    CoreLoop();
}

void DamnedCaverns::Init() {
    SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    is_running_ = true;
    SDL_ShowWindow(window_.get());
}

void DamnedCaverns::CoreLoop() {
    Uint64 previous_time = SDL_GetTicks64();
    Uint64 accumulated_time = 0;

    while (is_running_) {
        Uint64 current_time = SDL_GetTicks64();
        Uint64 frame_time = current_time - previous_time;
        previous_time = current_time;
        accumulated_time += frame_time;

        HandleEvents();

        while (accumulated_time >= kFixedTimeStep) {
            Update(kFixedTimeStep / 1000.0f);
            accumulated_time -= kFixedTimeStep;
        }

        Render();

        Uint64 frame_end = SDL_GetTicks64();
        Uint64 frame_duration = frame_end - current_time;
        if (frame_duration < kFrameDelay) {
            SDL_Delay(static_cast<Uint32>(kFrameDelay - frame_duration));
        }
    }
}

void DamnedCaverns::Update(float dt) {
    elapsed_time_ += dt;
    player_.Update(dt, grid_);
}

void DamnedCaverns::Render() {
    auto* renderer = renderer_.get();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Custom render
    RenderVisualHelper();

    // End Render
    SDL_RenderPresent(renderer);
}

void DamnedCaverns::RenderVisualHelper() {
    auto* renderer = renderer_.get();
    // Render cells
    const auto& cells = grid_.Cells();
    SDL_SetRenderDrawColor(renderer_.get(), 255, 0, 0, 255);
    for (const auto& cell : cells) {
        const SDL_FRect rect {
            cell.top_left_.x,
            cell.top_left_.y,
            kCellDimensionsF,
            kCellDimensionsF
        };

        if (cell.is_walkable_) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }

        SDL_RenderFillRectF(renderer, &rect);
    }

    // Render Player
    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 255, 255);
    const SDL_FRect player_rect {
        player_.position_.x, player_.position_.y, kPlayerDimensions, kPlayerDimensions
    };
    SDL_RenderFillRectF(renderer, &player_rect);

    // Render Monster
    SDL_SetRenderDrawColor(renderer_.get(), 255, 0, 0, 255);
    const auto monster_pos = grid_.ColRowToCoords(kMonsterStartColRow);
    const auto monster_rect = SDL_FRect {
        monster_pos.x, monster_pos.y, kCellDimensionsF, kCellDimensionsF
    };
    SDL_RenderFillRectF(renderer, &monster_rect);

    // Render Exit
    SDL_SetRenderDrawColor(renderer_.get(), 0, 255, 0, 255);
    const auto exit_pos = grid_.ColRowToCoords(kExitColRow);
    const auto exit_rect = SDL_FRect {
        exit_pos.x, exit_pos.y, kCellDimensionsF, kCellDimensionsF
    };
    SDL_RenderFillRectF(renderer, &exit_rect);
}

void DamnedCaverns::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Shutdown();
            return;
        }
    }
}

void DamnedCaverns::Shutdown() {
    is_running_ = false;
}
