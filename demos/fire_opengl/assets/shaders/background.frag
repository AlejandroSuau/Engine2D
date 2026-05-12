#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

void main() {
    vec2 uv = vUV;

    // =========================
    // Color base pared
    // =========================
    vec3 wall = vec3(0.06, 0.05, 0.05);

    // =========================
    // Líneas horizontales
    // =========================
    float lines = sin(uv.y * 55.0);

    float lineMask =
        smoothstep(0.93, 1.0, lines);

    wall += lineMask * vec3(0.08, 0.05, 0.03);

    // =========================
    // Distancia al fuego
    // =========================
    vec2 firePos = vec2(0.5, 0.18);

    float dist =
        distance(uv, firePos);

    // =========================
    // Luz naranja del fuego
    // =========================
    float glow =
        1.0 - smoothstep(0.0, 0.55, dist);

    vec3 fireLight =
        vec3(1.0, 0.42, 0.08);

    wall += fireLight * glow * 0.35;

    // =========================
    // Oscurecer bordes
    // =========================
    float vignette =
        distance(uv, vec2(0.5));

    wall *= 1.0 - vignette * 0.55;

    FragColor = vec4(wall, 1.0);
}