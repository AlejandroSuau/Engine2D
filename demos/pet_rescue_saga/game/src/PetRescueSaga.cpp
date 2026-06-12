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
    grid_.Update(dt);
}

void PetRescueSaga::Render() {
    auto* renderer = renderer_.get();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render
    // Grid blocks
    const auto& cells = grid_.Cells();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (const auto& cell : cells) {
        if (cell.block) {
            SDL_FRect rect_block {
                cell.block->position_.x, cell.block->position_.y, kCellDimensionsF, kCellDimensionsF};
            SDL_RenderFillRectF(renderer, &rect_block);
        }
    }

    // Grid cells
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 100);
    for (const auto& cell : cells) {
        SDL_FRect rect_cell {
            cell.top_left_coords_.x, cell.top_left_coords_.y, kCellDimensionsF, kCellDimensionsF};
        SDL_RenderDrawRectF(renderer, &rect_cell);
    }

    // End Render
    SDL_RenderPresent(renderer);
}

void PetRescueSaga::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Shutdown();
            return;
        }

        if (grid_.IsAnyBlockMoving()) {
            return;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_LEFT) {
            const Vec2<float> click_coords {
                static_cast<float>(event.button.x),
                static_cast<float>(event.button.y)};
            HandleLeftMouseButtonClick(click_coords);
        }
    }
}

void PetRescueSaga::HandleLeftMouseButtonClick(const Vec2<float>& coords) {
    auto* cell = grid_.GetCell(grid_.CoordsToColRow(coords));
    if (!cell) {
        return;
    }

    if (cell->block) {
        auto block_destination = cell->block->position_;
        const auto cell_col_row = cell->col_row_;
        cell->block.reset();

        // Everything above goes down
        for (auto row = cell_col_row.y; row >= 0; --row) {
            auto* current_cell = grid_.GetCell(ColRow_t{cell_col_row.x, row});
            // Always should be cell up
            if (current_cell->block) {
                current_cell->block->destination_ = block_destination;
                current_cell->block->is_moving_.y = true;
                block_destination = current_cell->block->position_;

                cell->block = std::move(current_cell->block);
                current_cell->block.reset();
                cell = current_cell;
            }
        }

    }

    // Which block?
    // Update the grid if something destroyed

}

void PetRescueSaga::CheckGameOver() {
}

void PetRescueSaga::Shutdown() {
    is_running_ = false;
}

void PetRescueSaga::ResetGame() {
    elapsed_time_ = 0.0f;
}
