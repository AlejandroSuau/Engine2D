#pragma once

#include <vector>
#include <memory>

#include "Utils.hpp"
#include "VisualEffect.hpp"
#include "Constants.hpp"

// Clase encargada de gestionar las partes de la demo.
class DemoController {
public:
    DemoController(int width, int height)
        : width_(width), height_(height) {}

    void AddEffect(std::unique_ptr<IVisualEffect> effect, float start_time, float end_time) {
        effect->Init(width_, height_);
        parts_.push_back({std::move(effect), start_time, end_time});
    }

    void Update(float dt, float total_demo_time) {
        IVisualEffect* active = GetActiveEffect(total_demo_time);
        if (!active) return;
        active->Update(total_demo_time);
    }

    // Se renderiza el efecto activo si lo hay en dicho periodo de tiempo y siempre
    // se aplican las transiciones globales.
    void Render(framebuffer_t& framebuffer, float total_demo_time) {
        if (auto active = GetActiveEffect(total_demo_time)) {
            active->Render(framebuffer, width_, height_);
        }
        
        ApplyGlobalTransitions(framebuffer, total_demo_time);
    }

private:
    struct DemoPart {
        std::unique_ptr<IVisualEffect> effect;
        float start_time;
        float end_time;
    };

    int width_, height_;
    std::vector<DemoPart> parts_;

    // Devuelve un efecto comprendido en un tiempo dado o nullptr si no se encuentra.
    IVisualEffect* GetActiveEffect(float t) {
        for (auto& part : parts_) {
            if (t >= part.start_time && t < part.end_time) {
                return part.effect.get();
            }
        }
        return nullptr;
    }

    void ApplyGlobalTransitions(framebuffer_t& framebuffer, float demo_time) {
        // Fade in / Fade out entre transiciones para cada parte de la demo.
        if (demo_time < 5.0f) {
            float alpha = 1.0f - SmoothStep(0.0f, 3.0f, demo_time);
            ApplyFade(framebuffer, alpha);
        }

        if (demo_time >= 13.0f && demo_time < 14.0f) {
            float alpha = SmoothStep(13.0f, 14.0f, demo_time);
            ApplyFade(framebuffer, alpha);
        }
        if (demo_time >= 14.0f && demo_time < 15.0f) {
            float alpha = 1.0f - SmoothStep(14.0f, 15.0f, demo_time);
            ApplyFade(framebuffer, alpha);
        }

        if (demo_time >= 33.0f && demo_time < 34.0f) {
            float alpha = SmoothStep(33.0f, 34.0f, demo_time);
            ApplyFade(framebuffer, alpha);
        }
        if (demo_time >= 34.0f && demo_time < 35.0f) {
            float alpha = 1.0f - SmoothStep(34.0f, 35.0f, demo_time);
            ApplyFade(framebuffer, alpha);
        }

        if (demo_time > 36.0f) {
            float alpha = SmoothStep(37.0f, 40.0f, demo_time);
            ApplyFade(framebuffer, alpha);
        }

        // Se activa el white flash solo en el 8% inicial de cada beat (y solamente para 
        // la parte del efecto del logo).
        float beat_phase = std::fmod(demo_time, kBeatDuration) / kBeatDuration;
        if (beat_phase < 0.08f) {
            float flash = 1.0f - (beat_phase / 0.08f);
            if (demo_time >= 37.0f) {
                const auto flash_intensity = 0.08f;
                ApplyWhiteFlash(framebuffer, flash * flash_intensity);
            }
        }
    }
};