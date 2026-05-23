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

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    SDL_GLContext gl_context_;

    int window_width_;
    int window_height_;

    float elapsed_time_ = 0.0f;
    bool is_running_;

    // Shaders
    Shader base_shader_;
    Shader fire_shader_;
    Shader heat_shader_;

    // Framebuffer intermedio
    GLuint scene_fbo_ = 0;
    GLuint scene_texture_ = 0;
    GLuint scene_rbo_ = 0;

    // Quad para renderizar el shader del fuego
    GLuint fire_vao_ = 0;
    GLuint fire_vbo_ = 0;
    GLuint fire_ebo_ = 0;

    // Quad utilizado para los efectos que ocupan toda la pantalla 
    // (distorsión de calor y escena base)
    GLuint screen_vao_ = 0;
    GLuint screen_vbo_ = 0;
    GLuint screen_ebo_ = 0;

    void ConfigureOpenGLAttributes();
    void CreateWindow();
    void CreateOpenGLContext();
    void Init();

    void SetupSceneFramebuffer();
    void SetupFireQuad();
    void RenderFireQuad(float offset_x, float scale);
    void SetupScreenQuad();

    void CoreLoop();
    void Update(float dt);
    void Render();
    void HandleEvents();
    void Shutdown();

    void RenderHeatQuad();
    void RenderBaseQuad(float offset_x, float offset_y, float scale_x, float scale_y);
    
};