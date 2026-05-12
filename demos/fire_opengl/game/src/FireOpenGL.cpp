#include "FireOpenGL.hpp"

FireOpenGL::FireOpenGL(int window_width, int window_height)
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

FireOpenGL::~FireOpenGL() {
    if (scene_rbo_ != 0) {
        glDeleteRenderbuffers(1, &scene_rbo_);
    }

    if (scene_texture_ != 0) {
        glDeleteTextures(1, &scene_texture_);
    }

    if (scene_fbo_ != 0) {
        glDeleteFramebuffers(1, &scene_fbo_);
    }

    if (quad_ebo_ != 0) {
        glDeleteBuffers(1, &quad_ebo_);
    }

    if (quad_vbo_ != 0) {
        glDeleteBuffers(1, &quad_vbo_);
    }

    if (quad_vao_ != 0) {
        glDeleteVertexArrays(1, &quad_vao_);
    }
    
    if (gl_context_ != nullptr) {
        SDL_GL_DeleteContext(gl_context_);
        gl_context_ = nullptr;
    }
}

void FireOpenGL::Run() {
    Init();
    CoreLoop();
}

void FireOpenGL::SetupSceneFramebuffer() {
    // Crear framebuffer
    glGenFramebuffers(1, &scene_fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo_);

    // Crear textura color donde se renderizará la escena
    glGenTextures(1, &scene_texture_);
    glBindTexture(GL_TEXTURE_2D, scene_texture_);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        window_width_,
        window_height_,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        scene_texture_,
        0
    );

    // Crear renderbuffer para profundidad y stencil
    glGenRenderbuffers(1, &scene_rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, scene_rbo_);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        window_width_,
        window_height_
    );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        scene_rbo_
    );

    // Verificar que el framebuffer está completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        throw std::runtime_error("Scene framebuffer is not complete");
    }

    // Desbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FireOpenGL::ConfigureOpenGLAttributes() {
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

void FireOpenGL::CreateWindow() {
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

void FireOpenGL::CreateOpenGLContext() {
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

void FireOpenGL::SetupFireQuad() {
    // Cada vértice: posición (x, y, z) + UV (u, v)
    const float vertices[] = {
        // x      y      z      u     v
        -0.3f, -0.6f, 0.0f,   0.0f, 0.0f, // abajo izquierda
        0.3f, -0.6f, 0.0f,   1.0f, 0.0f, // abajo derecha
        0.3f,  0.6f, 0.0f,   1.0f, 1.0f, // arriba derecha
        -0.3f,  0.6f, 0.0f,   0.0f, 1.0f  // arriba izquierda
    };

    const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &quad_vao_);
    glGenBuffers(1, &quad_vbo_);
    glGenBuffers(1, &quad_ebo_);

    glBindVertexArray(quad_vao_);

    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // location 0 -> posición (vec3)
    glVertexAttribPointer(
        0,                  // índice del atributo
        3,                  // número de componentes
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),  // stride total
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // location 1 -> UV (vec2)
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void FireOpenGL::Init() {
    SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    glViewport(0, 0, window_width_, window_height_);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fire_shader_.Load("assets/shaders/fire.vert", "assets/shaders/fire.frag");
    heat_shader_.Load("assets/shaders/heat.vert", "assets/shaders/heat.frag");
    //base_shader_.Load("assets/shaders/base.vert", "assets/shaders/base.frag");

    SetupFireQuad();
    SetupSceneFramebuffer();

    is_running_ = true;
    SDL_ShowWindow(window_.get());
}

void FireOpenGL::CoreLoop() {
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

void FireOpenGL::Update(float dt) {
    elapsed_time_ += dt;
}

void FireOpenGL::Render() {
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo_);
    glViewport(0, 0, window_width_, window_height_);
    glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderFireQuad(0.0f, 1.15f);
    RenderFireQuad(-0.06f, 0.90f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_width_, window_height_);
    glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderHeatQuad();

    RenderFireQuad(0.0f, 1.15f);
    RenderFireQuad(-0.06f, 0.90f);

    SDL_GL_SwapWindow(window_.get());
}

void FireOpenGL::RenderHeatQuad() {
    heat_shader_.Use();
    heat_shader_.SetFloat("uTime", elapsed_time_);
    heat_shader_.SetInt("uSceneTexture", 0);
    heat_shader_.SetVec2("uResolution", static_cast<float>(window_width_), static_cast<float>(window_height_));
    heat_shader_.SetFloat("uOffsetX", 0.0f);
    heat_shader_.SetFloat("uScale", 1.45f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, scene_texture_);

    glBindVertexArray(quad_vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void FireOpenGL::RenderFireQuad(float offset_x, float scale) {
    fire_shader_.Use();
    fire_shader_.SetFloat("uTime", elapsed_time_);
    fire_shader_.SetFloat("uOffsetX", offset_x);
    fire_shader_.SetFloat("uScale", scale);
    
    glBindVertexArray(quad_vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void FireOpenGL::RenderBaseQuad(float offset_x, float offset_y, float scale_x, float scale_y) {
    base_shader_.Use();
    base_shader_.SetFloat("uOffsetX", offset_x);
    base_shader_.SetFloat("uOffsetY", offset_y);
    base_shader_.SetFloat("uScaleX", scale_x);
    base_shader_.SetFloat("uScaleY", scale_y);

    // Color oscuro cálido
    base_shader_.SetVec3("uColor", 0.30f, 0.18f, 0.10f);

    glBindVertexArray(quad_vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void FireOpenGL::HandleEvents() {
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

void FireOpenGL::Shutdown() {
    is_running_ = false;
}
