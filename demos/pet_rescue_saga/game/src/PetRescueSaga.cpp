#include "PetRescueSaga.hpp"

#include "Constants.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

PetRescueSaga::PetRescueSaga(int window_width, int window_height)
    : sdl_initializer_(std::make_unique<SDLInitializer>())
    , window_width_(window_width)
    , window_height_(window_height)
    , window_(nullptr, SDL_DestroyWindow)
    , renderer_(nullptr, SDL_DestroyRenderer)
    , is_running_(false)
    , grid_(kGridRowCount, kGridColumnCount) {
    CreateWindow();
}

PetRescueSaga::~PetRescueSaga() {
}

void PetRescueSaga::CreateWindow() {
    window_.reset(SDL_CreateWindow(
        "Pet Rescue Saga - Alejandro Suau Ruiz",
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

void PetRescueSaga::Run() {
    Init();
    CoreLoop();
}

void PetRescueSaga::Init() {
    SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    is_running_ = true;
    SDL_ShowWindow(window_.get());
}

void PetRescueSaga::CoreLoop() {
    Uint64 previous_time = SDL_GetTicks64();
    Uint64 accumulated_time = 0;

    while (is_running_) {
        const Uint64 current_time = SDL_GetTicks64();
        const Uint64 frame_time = current_time - previous_time;
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

void PetRescueSaga::Update(float dt) {
    elapsed_time_ += dt;
}

void PetRescueSaga::Render() {
    auto* renderer = renderer_.get();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float current_x = 0.f;
    float current_y = 0.f;
    const auto& cells = grid_.GetCells();
    for (const auto& cell : cells) {
        current_x = static_cast<float>(cell.col) * kCellDimensionsF;
        current_y = static_cast<float>(cell.row) * kCellDimensionsF;
        SDL_FRect r {current_x, current_y, kCellDimensionsF, kCellDimensionsF};
        SDL_RenderDrawRectF(renderer, &r);
    }

    // End Render
    SDL_RenderPresent(renderer);
}

/*void PetRescueSaga::RenderVisualHelper() {
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
    const auto player_position = grid_.ColRowToCoords(player_.colrow_);
    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 255, 255);
    const SDL_FRect player_rect {
        player_position.x, player_position.y, kPlayerDimensions, kPlayerDimensions
    };
    SDL_RenderFillRectF(renderer, &player_rect);
    
    // Render Player Direction
    const auto player_center_x = player_position.x + kPlayerDimensions * 0.5f;
    const auto player_center_y = player_position.y + kPlayerDimensions * 0.5f;

    SDL_SetRenderDrawColor(renderer_.get(), 255, 0, 0, 255);
    SDL_RenderDrawLineF(
        renderer,
        player_center_x,
        player_center_y,
        player_center_x + player_.direction_.x * kCellDimensionsF,
        player_center_y + player_.direction_.y * kCellDimensionsF
    );

    // Render Monster
    SDL_SetRenderDrawColor(renderer_.get(), 255, 0, 0, 255);
    const auto monster_pos = grid_.ColRowToCoords(monster_.colrow_);
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
}*/

void PetRescueSaga::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Shutdown();
            return;
        }
    }
}

void PetRescueSaga::CheckGameOver() {
}

void PetRescueSaga::Shutdown() {
    is_running_ = false;
}

void PetRescueSaga::ResetGame() {
    elapsed_time_ = 0.0f;
}
