#include "SDLInitializer.hpp"
#include "Constants.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include <stdexcept>
#include <memory>
#include <string>

class FireOpenGL {
public:
    FireOpenGL(int window_width, int window_height)
        : sdl_initializer_(std::make_unique<SDLInitializer>())
        , window_width_(window_width)
        , window_height_(window_height)
        , window_(nullptr, SDL_DestroyWindow)
        , gl_context_(nullptr)
        , is_running_(false)
    {
        ConfigureOpenGLAttributes();
        CreateWindow();
        CreateOpenGLContext();
    }

    ~FireOpenGL() {
        if (gl_context_ != nullptr) {
            SDL_GL_DeleteContext(gl_context_);
            gl_context_ = nullptr;
        }
    }

    void Run() {
        Init();
        CoreLoop();
    }

private:
    std::unique_ptr<SDLInitializer> sdl_initializer_;

    int window_width_;
    int window_height_;

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    SDL_GLContext gl_context_;

    bool is_running_;

    void ConfigureOpenGLAttributes() {
        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
            throw std::runtime_error(std::string("Error setting OpenGL profile: ") + SDL_GetError());
        }

        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) != 0) {
            throw std::runtime_error(std::string("Error setting OpenGL major version: ") + SDL_GetError());
        }

        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) != 0) {
            throw std::runtime_error(std::string("Error setting OpenGL minor version: ") + SDL_GetError());
        }

        if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0) {
            throw std::runtime_error(std::string("Error setting double buffer: ") + SDL_GetError());
        }

        if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0) {
            throw std::runtime_error(std::string("Error setting depth buffer size: ") + SDL_GetError());
        }
    }

    void CreateWindow() {
        window_.reset(SDL_CreateWindow(
            "FireOpenGL - Alejandro Suau Ruiz",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width_,
            window_height_,
            SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI
        ));

        if (!window_) {
            throw std::runtime_error(std::string("Error creating window: ") + SDL_GetError());
        }
    }

    void CreateOpenGLContext() {
        gl_context_ = SDL_GL_CreateContext(window_.get());
        if (gl_context_ == nullptr) {
            throw std::runtime_error(std::string("Error creating OpenGL context: ") + SDL_GetError());
        }

        if (SDL_GL_MakeCurrent(window_.get(), gl_context_) != 0) {
            throw std::runtime_error(std::string("Error making OpenGL context current: ") + SDL_GetError());
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Error initializing GLAD");
        }

        const GLubyte* version = glGetString(GL_VERSION);
        if (version) {
            SDL_Log("OpenGL version: %s", version);
        }

        if (SDL_GL_SetSwapInterval(1) != 0) {
            throw std::runtime_error(std::string("Error enabling VSync: ") + SDL_GetError());
        }
    }

    void Init() {
        SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        glViewport(0, 0, window_width_, window_height_);
        glEnable(GL_DEPTH_TEST);

        is_running_ = true;
        SDL_ShowWindow(window_.get());
    }

    void CoreLoop() {
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

    void Update(float dt) {
        (void)dt;
    }

    void Render() {
        glClearColor(0.08f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window_.get());
    }

    void HandleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                Shutdown();
                return;
            }

            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
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