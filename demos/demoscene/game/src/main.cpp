#include "SDLInitializer.hpp"
#include "Constants.hpp"
#include "Utils.hpp"
#include "VisualEffect.hpp"
#include "DemoController.hpp"

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <stdexcept>
#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>

//////////////////////// DEMO SCENE ////////////////////////////////////
class DemoScene {
public:
    DemoScene(int window_width, int window_height) 
        : sdl_initializer_(std::make_unique<SDLInitializer>())
        , sdl_initializer_image_(std::make_unique<SDLImageInitializer>())
        , sdl_initializer_mixer_(std::make_unique<SDLMixerInitializer>())
        , window_width_(window_width)
        , window_height_(window_height)
        , window_(
            SDL_CreateWindow(
                "DemoScene - Alejandro Suau Ruiz",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                window_width_,
                window_height_,
                SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI),
            SDL_DestroyWindow)
        , renderer_(
            SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED),
            SDL_DestroyRenderer)
        , texture_(nullptr, SDL_DestroyTexture)
        , music_(nullptr, Mix_FreeMusic)
        , is_running_(false)
        , demo_controller_(window_width_, window_height_)
        , demo_time_(0.f)
    {
        if (!window_ || !renderer_) {
            throw std::runtime_error(
                std::string("Error creating the DemoScene") + SDL_GetError());
        }
    }

    void Run() {
        Init();
        CoreLoop();
    }

private:
    std::unique_ptr<SDLInitializer> sdl_initializer_;
    std::unique_ptr<SDLImageInitializer> sdl_initializer_image_;
    std::unique_ptr<SDLMixerInitializer> sdl_initializer_mixer_;
    
    int window_width_;
    int window_height_;
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> texture_;
    std::unique_ptr<Mix_Music, void(*)(Mix_Music*)> music_;
    framebuffer_t framebuffer_;
    bool is_running_;

    DemoController demo_controller_;
    float demo_time_;

    void Init() {
        SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        texture_.reset(SDL_CreateTexture(
            renderer_.get(),
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window_width_,
            window_height_
        ));
        SDL_SetTextureScaleMode(texture_.get(), SDL_ScaleModeNearest);
        if (!texture_) {
            throw std::runtime_error(std::string("CreateTexture failed: ") + SDL_GetError());
        }
        framebuffer_.resize(static_cast<size_t>(window_width_ * window_height_), 0xFF000000u);

        demo_controller_.AddEffect(std::make_unique<TunnelEffect>(), 0.0f, 14.0f);
        demo_controller_.AddEffect(std::make_unique<RotozoomEffect>(), 14.0f, 34.0f);
        demo_controller_.AddEffect(std::make_unique<LogoEffect>(), 34.0f, 40.0f);

        music_.reset(Mix_LoadMUS("assets/sound/bg-music.mp3"));
        if (!music_) {
            SDL_Log("No se pudo cargar music.ogg: %s", Mix_GetError());
        } else {
            Mix_PlayMusic(music_.get(), 1);
        }

        is_running_ = true;
        SDL_ShowWindow(window_.get());
    }

    void CoreLoop() {
        // Core loop independiente de la potencia de la máquina.
        // (Inspirado por mi propio game engine)
        Uint64 previous_time = SDL_GetTicks64();
        Uint64 accumulated_time = 0;
        while (is_running_) {
            Uint64 current_time = SDL_GetTicks64();
            Uint64 frame_time = current_time - previous_time;
            previous_time = current_time;
            accumulated_time += frame_time;

            HandleEvents();

            // Fixed Update Loop
            while (accumulated_time >= kFixedTimeStep) {
                Update(kFixedTimeStep / 1000.f);
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

    void Update(float dt) {
        demo_controller_.Update(dt, demo_time_);
        demo_time_ += dt;

        if (demo_time_ >= kDemoTimeLimit) {
            Shutdown();
        }
    }

    void Render() {
        Clear(framebuffer_, 0xFF000000u);

        demo_controller_.Render(framebuffer_, demo_time_);

        const int pitch = window_width_ * static_cast<int>(sizeof(uint32_t));
        if (SDL_UpdateTexture(texture_.get(), nullptr, framebuffer_.data(), pitch) != 0) {
            SDL_Log("SDL_UpdateTexture error: %s", SDL_GetError());
        }

        SDL_RenderClear(renderer_.get());
        SDL_RenderCopy(renderer_.get(), texture_.get(), nullptr, nullptr);
        SDL_RenderPresent(renderer_.get());
    }

    void HandleEvents() {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                Shutdown();
                return;
            }
        }
    }

    void Shutdown() {
        is_running_ = false;
    }
};


int main(int argc, char** argv) {
    DemoScene demo_scene(SCREEN_WIDTH, SCREEN_HEIGHT);
    demo_scene.Run();

    return 0;
}
