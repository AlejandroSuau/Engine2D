#pragma once

#include <SDL_image.h>

#include "Constants.hpp"
#include "Utils.hpp"

#include <cmath>
#include <stdexcept>
#include <cassert>
#include <numbers>

class IVisualEffect {
public:
    virtual ~IVisualEffect() = default;
    virtual void Init(int width, int height) = 0;
    virtual void Update(float total_demo_time) = 0;
    virtual void Render(framebuffer_t& framebuffer, int width, int height) = 0;
};

class TunnelEffect : public IVisualEffect {
public:
    void Init(int width, int height) override {
        width_ = width;
        height_ = height;

        uv_.resize(width_ * height_);
        texture_.resize(kTexSize * kTexSize);

        // Textura procedural
        for (int y = 0; y < kTexSize; ++y) {
            for (int x = 0; x < kTexSize; ++x) {
                float wave = (x + y) * 0.05f;
                const auto r = static_cast<uint8_t>((std::sin(wave) * 0.5f + 0.5f) * 120);
                const auto g = static_cast<uint8_t>((std::sin(wave) * 0.5f + 0.5f) * 40);
                const auto b = static_cast<uint8_t>((std::sin(wave) * 0.5f + 0.5f) * 255);
                texture_[y * kTexSize + x] = RGB(r, g, b);
            }
        }

        // Precalcular U,V por píxel
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                // Recentrar las coordenadas para el centro del tunel.
                float dx = x - width_ * 0.5f;
                float dy = y - height_ * 0.5f;
                
                // Ángulo respecto al centro. Para rodear el tunel
                float angle = std::atan2(dy, dx);
                float dist = std::max(std::hypot(dx, dy), 1.0f);

                uint8_t u = static_cast<uint8_t>(
                    ((angle / (2.0f * std::numbers::pi_v<float>)) + 0.5f) * 255.0f
                );
                uint8_t v = static_cast<uint8_t>(4096.0f / dist);

                uv_[y * width_ + x] = {u, v};
            }
        }
    }

    void Update(float total_demo_time) override {
        time_ = total_demo_time;
    }

    void Render(framebuffer_t& framebuffer, int width, int height) override {
        const auto du = static_cast<uint8_t>(time_ * 120.0f);
        const auto dv = static_cast<uint8_t>(time_ * 220.0f);

        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                const auto& uv = uv_[y * width_ + x];
                const uint8_t u = uv.u + du;
                const uint8_t v = uv.v + dv;
                framebuffer[y * width_ + x] = texture_[v * kTexSize + u];
            }
        }
    }

private:
    static constexpr int kTexSize = 256;

    struct UV {
        uint8_t u;
        uint8_t v;
    };

    int width_ = 0;
    int height_ = 0;
    float time_ = 0.0f;
    std::vector<UV> uv_;
    framebuffer_t texture_;
};

// Efecto de rotación con zoom in out sobre la textura cuadriculada
class RotozoomEffect : public IVisualEffect {
public:
    void Init(int width, int height) override {
        width_ = width;
        height_ = height;
        texture_.resize(kTexSize * kTexSize);

        // Crea una rejilla con ondas sinusoidales
        for (int y = 0; y < 256; ++y) {
            for (int x = 0; x < 256; ++x) {
                // Crea ondas sinusoidales
                float v = std::sin(x * 0.08f) * std::sin(y * 0.08f);
                uint8_t c = static_cast<uint8_t>(v * 127.0f + 128.0f);

                // Tinte azulado con gradiente
                uint8_t r = c / 2u;
                uint8_t g = c / 6u;
                uint8_t b = c;

                texture_[y * kTexSize + x] = RGB(r, g, b);
            }
        }
    }

    void Update(float total_demo_time) override {
        time_ = total_demo_time;
    }

    void Render(framebuffer_t& framebuffer, int width, int height) override {
        // Rota en función del tiempo y hace zoom in/out con una oscilación
        const float angle = time_ * 0.8f;
        const float zoom = std::max(0.001f, 0.8f + 0.4f * std::sin(time_ * 2.0f));

        const float cs = std::cos(angle) / zoom;
        const float sn = std::sin(angle) / zoom;

        const float cx = 128.0f + 64.0f * std::sin(time_ * 0.7f);
        const float cy = 128.0f + 64.0f * std::cos(time_ * 0.9f);

        const float half_w = width_ * 0.5f;
        const float half_h = height_ * 0.5f;

        for (int y = 0; y < height_; ++y) {
            float py = y - half_h;

            for (int x = 0; x < width_; ++x) {
                float px = x - half_w;

                float u = cx + px * cs - py * sn;
                float v = cy + px * sn + py * cs;

                const auto tex_index = (
                    static_cast<int>(v) & kTexMask) * kTexSize + (static_cast<int>(u) & kTexMask);
                framebuffer[y * width_ + x] = texture_[tex_index];
            }
        }
    }

private:
    static constexpr int kTexSize = 256;
    static constexpr int kTexMask = 255;

    int width_ = 0;
    int height_ = 0;
    float time_ = 0.0f;
    framebuffer_t texture_;
};

class LogoEffect : public IVisualEffect {
public:
    void Init(int width, int height) override {
        width_ = width;
        height_ = height;

        SDL_Surface* loaded = IMG_Load("assets/images/uoc_demo_logo.png");
        if (!loaded) {
            throw std::runtime_error(
                std::string("No se pudo cargar uoc_demo_logo.png: ") + IMG_GetError()
            );
        }

        // Obtiene la imagen píxel por píxel
        SDL_Surface* converted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(loaded);

        if (!converted) {
            throw std::runtime_error(
                std::string("No se pudo convertir el PNG: ") + SDL_GetError()
            );
        }

        logo_w_ = converted->w;
        logo_h_ = converted->h;

        // Copia los píxeles de la imagen
        uint32_t* pixels = static_cast<uint32_t*>(converted->pixels);
        logo_pixels_.assign(pixels, pixels + logo_w_ * logo_h_);

        SDL_FreeSurface(converted);
    }

    void Update(float total_demo_time) override {
        time_ = total_demo_time;
    }

    void Render(framebuffer_t& framebuffer, int width, int height) override {
        // Aparición progresiva mientras asciende ligeramente.
        const float enter = SmoothStep(0.0f, 1.5f, time_);
        const float base_y = height_ * 0.54f - 70.0f * enter;

        // Pequeña oscilación vertical.
        const float sinus = std::sin(time_ * 2.8f) * 6.0f;
        const float y = base_y + sinus;

        // Sincronización con el beat.
        const float beat_phase = std::fmod(time_, kBeatDuration) / kBeatDuration;

        // Pequeño pulso visual.
        float beat_pulse = 0.0f;
        if (beat_phase < 0.18f) {
            beat_pulse = 1.0f - (beat_phase / 0.18f);
        }

        // Escala animada con zoom y sincronización con el beat.
        const float scale = 0.72f
                          + 0.03f * std::sin(time_ * 1.6f)
                          + 0.025f * beat_pulse;

        // Opacidad animada con pequeño incremento en cada beat y atenuando
        // al principio para la entrada.
        float opacity = 0.78f + 0.20f * beat_pulse;
        opacity *= (0.25f + 0.75f * enter);
        opacity = std::clamp(opacity, 0.0f, 1.0f);

        DrawScaledLogo(
            framebuffer,
            static_cast<int>(width_ * 0.5f),
            static_cast<int>(y),
            scale,
            opacity);
    }

private:
    int width_ = 0;
    int height_ = 0;
    int logo_w_ = 0;
    int logo_h_ = 0;
    float time_ = 0.f;
    framebuffer_t logo_pixels_;

    void DrawScaledLogo(
        framebuffer_t& framebuffer,
        int center_x,
        int center_y,
        float scale,
        float opacity) {
        const int dst_w = std::max(1, static_cast<int>(logo_w_ * scale));
        const int dst_h = std::max(1, static_cast<int>(logo_h_ * scale));

        const int start_x = center_x - dst_w / 2;
        const int start_y = center_y - dst_h / 2;

        const float inv_scale = 1.0f / scale;
        for (int dy = 0; dy < dst_h; ++dy) {
            // Fila destino a equivalente en el logo
            const int sy = static_cast<int>(dy * inv_scale);
            const auto py = start_y + dy;
            if (py < 0 || py >= height_) continue;

            for (int dx = 0; dx < dst_w; ++dx) {
                // Columna destino a equivalente en el logo
                const auto sx = static_cast<int>(dx * inv_scale);
                const int px = start_x + dx;
                if (px < 0 || px >= width_) continue;

                auto src = logo_pixels_[sy * logo_w_ + sx];

                // Modula el alpha original con el del efecto
                const auto a = static_cast<uint8_t>(A(src) * opacity);
                if (a == 0) continue;

                src = ARGB(a, R(src), G(src), B(src));

                // Si el pixel source es opaco, copia directo.
                auto& dst = framebuffer[py * width_ + px];
                if (a == 255) {
                    dst = src;
                } else {
                    dst = AlphaBlend(src, dst);
                }
            }
        }
    }
};
