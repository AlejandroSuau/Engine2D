#include "Engine2D.hpp"

#include <stdexcept>

namespace Engine2D {

namespace {
// Update
static constexpr Uint64 kTargetFPS = 60;
static constexpr Uint64 kFixedTimeStep = 1000 / kTargetFPS;
static constexpr Uint64 kFrameDelay = 1000 / kTargetFPS; // Frame delay in milliseconds
}

Engine::Engine()
    : sdl_(std::make_unique<SDLInitializer::Main>())
    , sdl_image_(std::make_unique<SDLInitializer::Image>())
    , sdl_mixer_(std::make_unique<SDLInitializer::Mixer>())
    , sdl_ttf_(std::make_unique<SDLInitializer::TTF>())
    , window_(
        SDL_CreateWindow(
            "Engine",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            0,
            0,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN),
        SDL_DestroyWindow)
    , sdl_renderer_(
        SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer)
    , is_running_(false) {
    if (!window_ || !sdl_renderer_) {
        throw std::runtime_error(
            std::string("Error creating the game") + SDL_GetError());
    }
    
    renderer_ = std::make_unique<Renderer>(*sdl_renderer_);

    SDL_SetRenderDrawBlendMode(sdl_renderer_.get(), SDL_BLENDMODE_BLEND);
}
    
void Engine::Run(std::unique_ptr<IGame> game) {
    is_running_ = true;
    game_ = std::move(game);
    game_->Start();

    Init();
    
    CoreLoop();
}

void Engine::Init() {
    SDL_SetWindowSize(window_.get(), game_->GetScreenWidth(), game_->GetScreenHeight());
    SDL_SetWindowTitle(window_.get(), game_->GetWindowTitle().c_str());
    SDL_ShowWindow(window_.get());
}

void Engine::CoreLoop() {
    Uint64 previous_time = SDL_GetTicks64();
    Uint64 accumulated_time = 0;
    while (is_running_) {
        Uint64 current_time = SDL_GetTicks64();
        Uint64 frame_time = current_time - previous_time; // Delta time in milliseconds
        previous_time = current_time;
        accumulated_time += frame_time;

        HandleEvents();

        // Fixed Update Loop
        while (accumulated_time >= kFixedTimeStep) {
            Update(kFixedTimeStep / 1000.f); // Pass in seconds as a float
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

void Engine::Update(float dt) {

}

void Engine::Render() {
    auto* renderer = sdl_renderer_.get();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render

    if (game_) {
        game_->Render(*renderer_);
    }
    
    // End Render

    SDL_RenderPresent(renderer);
}

void Engine::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Shutdown();
            return;
        }

        if (event.type == SDL_MOUSEBUTTONUP) {
        }

        if (event.type == SDL_KEYUP) {
        }
    }
}

void Engine::Shutdown() {
    is_running_ = false;
}

}
