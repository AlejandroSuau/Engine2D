#pragma once

#include "Constants.hpp"

#include <algorithm>

// Funciones para cada canal por separado
static inline uint8_t A(uint32_t c) { return (c >> 24) & 0xFF; }
static inline uint8_t R(uint32_t c) { return (c >> 16) & 0xFF; }
static inline uint8_t G(uint32_t c) { return (c >> 8)  & 0xFF; }
static inline uint8_t B(uint32_t c) { return c & 0xFF; }

// Función para pasar de rgb a formato int de 32 bits (ARGB)
static inline uint32_t ARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return ((static_cast<uint32_t>(a) << 24) | 
            (static_cast<uint32_t>(r) << 16) |
            (static_cast<uint32_t>(g) << 8)  |
            static_cast<uint32_t>(b));
}

static inline uint32_t RGB(uint8_t r, uint8_t g, uint8_t b) {
    return ARGB(0xFF, r, g, b);
}

// Limpia el framebuffer con un color dado
static inline void Clear(framebuffer_t& fb, uint32_t color) {
    std::fill(fb.begin(), fb.end(), color);
}

// Modula el canal alpha multiplicándolo por un factor
static inline uint32_t ModulateAlpha(uint32_t color, float alpha_mul) {
    alpha_mul = std::clamp(alpha_mul, 0.0f, 1.0f);

    uint8_t a = (color >> 24) & 0xFF;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // Reducimos la opacidad
    a = static_cast<uint8_t>(a * alpha_mul);

    return (a << 24) | (r << 16) | (g << 8) | b;
}

// Mezcla alpha estándar (opaco)
// Fórmula: out = src * a + dst * (1 - a)
static inline uint32_t AlphaBlend(uint32_t src, uint32_t dst) {
    uint8_t sa = (src >> 24) & 0xFF;
    uint8_t sr = (src >> 16) & 0xFF;
    uint8_t sg = (src >> 8) & 0xFF;
    uint8_t sb = src & 0xFF;

    uint8_t dr = (dst >> 16) & 0xFF;
    uint8_t dg = (dst >> 8) & 0xFF;
    uint8_t db = dst & 0xFF;

    // Interpolación lineal por canal
    uint8_t r = static_cast<uint8_t>((sr * sa + dr * (255 - sa)) / 255);
    uint8_t g = static_cast<uint8_t>((sg * sa + dg * (255 - sa)) / 255);
    uint8_t b = static_cast<uint8_t>((sb * sa + db * (255 - sa)) / 255);

    return 0xFF000000u | (r << 16) | (g << 8) | b;
}

// Aplica un fade a negro oscureciendo el framebuffer
void ApplyFade(framebuffer_t& framebuffer, float alpha) {
    alpha = std::clamp(alpha, 0.0f, 1.0f);
    const float factor = 1.0f - alpha;

    for (auto& px : framebuffer) {
        const auto a = A(px);
        const auto r = static_cast<uint8_t>(R(px) * factor);
        const auto g = static_cast<uint8_t>(G(px) * factor);
        const auto b = static_cast<uint8_t>(B(px) * factor);
        px = ARGB(a, r, g, b);
    }
}

// Interpolación de Hermite
// Normaliza x y después aplica la curva smooth: 3x^2 - 2x^2
float SmoothStep(float edge0, float edge1, float x) {
    x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return x * x * (3.0f - 2.0f * x);
}

// Mezcla el framebuffer hacia blanco (para que parezca un efecto de flash: 1 sería blanco total)
void ApplyWhiteFlash(framebuffer_t& framebuffer, float alpha) {
    alpha = std::clamp(alpha, 0.0f, 1.0f);

    for (auto& px : framebuffer) {
        auto a = A(px);
        auto r = static_cast<uint8_t>(R(px));
        auto g = static_cast<uint8_t>(G(px));
        auto b = static_cast<uint8_t>(B(px));

        // Interpolación hacia 255 (blanco)
        r = static_cast<uint8_t>(r + (255 - r) * alpha);
        g = static_cast<uint8_t>(g + (255 - g) * alpha);
        b = static_cast<uint8_t>(b + (255 - b) * alpha);
        px = ARGB(a, r, g, b);
    }
}