#include "SDLInitializer.hpp"
#include "Constants.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>
#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>
#include <array>

class FireOpenGL {
public:
    FireOpenGL(int window_width, int window_height) 
        : sdl_initializer_(std::make_unique<SDLInitializer>())
        , sdl_initializer_image_(std::make_unique<SDLImageInitializer>())
        , sdl_initializer_mixer_(std::make_unique<SDLMixerInitializer>())
        , window_width_(window_width)
        , window_height_(window_height)
        , window_(
            SDL_CreateWindow(
                "FireOpenGL - Alejandro Suau Ruiz",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                window_width_,
                window_height_,
                SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI),
            SDL_DestroyWindow)
        , is_running_(false)
    {
        if (!window_) {
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
    bool is_running_;

    void Init() {
        SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

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

    }

    void Render() {

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
    FireOpenGL fire_opengl(SCREEN_WIDTH, SCREEN_HEIGHT);
    fire_opengl.Run();

    return 0;
}
