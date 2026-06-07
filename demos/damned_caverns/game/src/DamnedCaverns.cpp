#include "DamnedCaverns.hpp"

#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>

DamnedCaverns::DamnedCaverns(int window_width, int window_height)
    : sdl_initializer_(std::make_unique<SDLInitializer>())
    , sdl_initializer_mixer_(std::make_unique<SDLMixerInitializer>())
    , window_width_(window_width)
    , window_height_(window_height)
    , window_(nullptr, SDL_DestroyWindow)
    , renderer_(nullptr, SDL_DestroyRenderer)
    , is_running_(false)
    , audio_manager_()
    , game_state_(GameState::Playing)
    , grid_({0, 0}, kMapColumnCount, kMapRowCount, kCellDimensions)
    , player_{
        .direction_ = {-1, 0},
        .colrow_ = kPlayerStartColRow
    }
    , monster_{
        .colrow_ = kMonsterStartColRow
    }
{
    CreateWindow();
    SetMapData();
}

DamnedCaverns::~DamnedCaverns() {
    audio_manager_.Shutdown();
}

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
    auto& cells = grid_.Cells();
    for (auto& cell : cells) {
        const auto& colrow = cell.colrow_;
        const auto col = static_cast<std::size_t>(colrow.x);
        const auto row = static_cast<std::size_t>(colrow.y);
        cell.is_walkable_ = kMapData[row][col];
    }
}

void DamnedCaverns::Run() {
    Init();
    CoreLoop();
}

void DamnedCaverns::Init() {
    SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    if (!audio_manager_.Load()) {
        throw std::runtime_error("Failed to load assets");
    }

    monster_snore_channel_ = audio_manager_.PlayLoop(SoundId::MonsterSnore, 70);
    waterfall_channel_ = audio_manager_.PlayLoop(SoundId::Waterfall, 60);

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

    audio_manager_.Update(dt);

    if (game_state_ != GameState::Playing) {
        return;
    }

    audio_manager_.UpdateSpatialChannel(
        monster_snore_channel_,
        player_.colrow_,
        monster_.colrow_,
        player_.direction_
    );

    audio_manager_.UpdateSpatialChannel(
        waterfall_channel_,
        player_.colrow_,
        kExitColRow,
        player_.direction_
    );
}

void DamnedCaverns::Render() {
    auto* renderer = renderer_.get();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Custom render
    if (show_visual_helper_) {
        RenderVisualHelper();
    }

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
}

void DamnedCaverns::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Shutdown();
            return;
        }

        if (event.type != SDL_KEYDOWN || event.key.repeat != 0) {
            continue;
        }

        // Helpers para reiniciar y visualizar EL GRID
        switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_V: show_visual_helper_ = !show_visual_helper_; break;
            case SDL_SCANCODE_R: ResetGame(); break; 
        }

        if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
            if (game_state_ != GameState::Playing) {
                return;
            }

            bool did_player_face_wall = false;
            bool did_move = false;
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                    did_player_face_wall = !player_.MoveForward(grid_);
                    did_move = true;
                    break;

                case SDL_SCANCODE_S:
                    did_player_face_wall = !player_.MoveBackward(grid_);
                    did_move = true;
                    break;

                case SDL_SCANCODE_A:
                    player_.RotateLeft();
                    break;

                case SDL_SCANCODE_D:
                    player_.RotateRight();
                    break;

                default:
                    break;
            }

            if (did_move) {
                if (did_player_face_wall) {
                    audio_manager_.PlayOneShot(SoundId::WallHit, 90);
                    MoveMonsterRandomly();
                } else {
                    audio_manager_.PlayOneShot(SoundId::PlayerStep, 75);
                    audio_manager_.PlayDelayedOneShot(SoundId::PlayerStep, 0.10f, 30);
                    audio_manager_.PlayDelayedOneShot(SoundId::PlayerStep, 0.20f, 10);
                }
                CheckGameOver();
            }
        }
    }
}

void DamnedCaverns::MoveMonsterRandomly() {
    // Movimiento a casilla adyacente aleatoria.
    std::array<ColRow_t, 4> directions = {{
        {1, 0},   // Derecha
        {-1, 0},  // Izquierda
        {0, 1},   // Abajo
        {0, -1}   // Arriba
     }};
     std::shuffle(
        directions.begin(),
        directions.end(),
        std::mt19937{std::random_device{}()});
    
    for (const auto& dir : directions) {
        const ColRow_t next_colrow {
            monster_.colrow_.x + dir.x,
            monster_.colrow_.y + dir.y
        };

        if (grid_.AreColRowWalkable(next_colrow)) {
            monster_.colrow_ = next_colrow;
            audio_manager_.PlayOneShot(SoundId::MonsterStep, 80);
            audio_manager_.PlayDelayedOneShot(SoundId::MonsterStep, 0.14f, 28);
            return;
        }
    }
}

void DamnedCaverns::CheckGameOver() {
    if (player_.colrow_ == monster_.colrow_) {
        TriggerDefeat();
    } else if (player_.colrow_ == kExitColRow) {
        TriggerVictory();
    }
}

void DamnedCaverns::TriggerVictory() {
    audio_manager_.StopAll();
    audio_manager_.PlayOneShot(SoundId::Victory, 80);
    game_state_ = GameState::Victory;
}

void DamnedCaverns::TriggerDefeat() {
    audio_manager_.StopAll();
    audio_manager_.PlayOneShot(SoundId::MonsterEat, 60);
    audio_manager_.PlayDelayedOneShot(SoundId::MonsterEat, 0.30f, 45);
    audio_manager_.PlayDelayedOneShot(SoundId::MonsterEat, 0.80f, 24);
    audio_manager_.PlayDelayedOneShot(SoundId::MonsterEat, 1.1f, 10);

    audio_manager_.PlayDelayedOneShot(SoundId::Defeat, 1.1f, 80);
    game_state_ = GameState::Defeat;
}

void DamnedCaverns::Shutdown() {
    is_running_ = false;
}

void DamnedCaverns::StartAmbientLoops() {
    monster_snore_channel_ = audio_manager_.PlayLoop(SoundId::MonsterSnore, 70);
    waterfall_channel_ = audio_manager_.PlayLoop(SoundId::Waterfall, 60);

    audio_manager_.UpdateSpatialChannel(
        monster_snore_channel_,
        player_.colrow_,
        monster_.colrow_,
        player_.direction_
    );

    audio_manager_.UpdateSpatialChannel(
        waterfall_channel_,
        player_.colrow_,
        kExitColRow,
        player_.direction_
    );
}

void DamnedCaverns::ResetGame() {
    audio_manager_.StopAll();

    elapsed_time_ = 0.0f;
    game_state_ = GameState::Playing;

    player_.direction_ = {-1, 0};
    player_.colrow_ = kPlayerStartColRow;

    monster_.colrow_ = kMonsterStartColRow;

    StartAmbientLoops();
}
