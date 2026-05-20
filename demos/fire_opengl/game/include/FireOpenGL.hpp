#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "SDLInitializer.hpp"
#include "Constants.hpp"
#include "Shader.hpp"

#include <memory>

class FireOpenGL {
public:
    FireOpenGL(int window_width, int window_height);
    ~FireOpenGL();

    void Run();

private:
    std::unique_ptr<SDLInitializer> sdl_initializer_;

    int window_width_;
    int window_height_;

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    SDL_GLContext gl_context_;

    float elapsed_time_ = 0.0f;
    bool is_running_;

    Shader base_shader_;
    Shader fire_shader_;
    Shader heat_shader_;

    GLuint scene_fbo_ = 0;
    GLuint scene_texture_ = 0;
    GLuint scene_rbo_ = 0;

    GLuint fire_vao_ = 0;
    GLuint fire_vbo_ = 0;
    GLuint fire_ebo_ = 0;

    GLuint screen_vao_ = 0;
    GLuint screen_vbo_ = 0;
    GLuint screen_ebo_ = 0;

    void SetupSceneFramebuffer();
    void ConfigureOpenGLAttributes();
    void CreateWindow();
    void CreateOpenGLContext();
    void SetupFireQuad();
    void Init();
    void CoreLoop();
    void Update(float dt);
    void Render();
    void RenderHeatQuad();
    void RenderFireQuad(float offset_x, float scale);
    void RenderBaseQuad(float offset_x, float offset_y, float scale_x, float scale_y);
    void HandleEvents();
    void Shutdown();
    void SetupScreenQuad();
};