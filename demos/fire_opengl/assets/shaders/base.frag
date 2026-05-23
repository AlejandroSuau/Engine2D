#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

// Variables "artísticas" para evitar números mágicos en el código
const vec3 WALL_BASE_COLOR = vec3(0.045, 0.038, 0.035);

const float HORIZONTAL_LINE_FREQUENCY = 55.0;
const float HORIZONTAL_LINE_START = 0.91;
const float HORIZONTAL_LINE_END = 1.00;
const vec3 HORIZONTAL_LINE_COLOR = vec3(0.09, 0.055, 0.032);

const float VERTICAL_LINE_FREQUENCY = 22.0;
const float VERTICAL_LINE_START = 0.996;
const float VERTICAL_LINE_END = 1.00;
const vec3 VERTICAL_LINE_COLOR = vec3(0.015, 0.010, 0.008);

const vec2 FIRE_LIGHT_POSITION = vec2(0.5, 0.32);
const float FIRE_LIGHT_INNER_RADIUS = 0.04;
const float FIRE_LIGHT_OUTER_RADIUS = 0.62;
const vec3 FIRE_LIGHT_COLOR = vec3(1.0, 0.38, 0.07);
const float FIRE_LIGHT_INTENSITY = 0.45;

// Máscara para las líneas horizontales
float horizontalLineMask(vec2 uv) {
    float wave = sin(uv.y * HORIZONTAL_LINE_FREQUENCY);

    return smoothstep(
        HORIZONTAL_LINE_START,
        HORIZONTAL_LINE_END,
        wave
    );
}

// Máscara para las líneas verticales
float verticalLineMask(vec2 uv) {
    float wave = sin(uv.x * VERTICAL_LINE_FREQUENCY);

    return smoothstep(
        VERTICAL_LINE_START,
        VERTICAL_LINE_END,
        wave
    );
}

// Añade líneas de la pared al color base
vec3 applyWallLines(vec3 color, vec2 uv) {
    float horizontalLines = horizontalLineMask(uv);
    float verticalLines = verticalLineMask(uv);

    color += horizontalLines * HORIZONTAL_LINE_COLOR;
    color += verticalLines * VERTICAL_LINE_COLOR;

    return color;
}

// Calcula la zona iluminada por el fuego (desvaneciendo el radio exterior)
float fireGlowMask(vec2 uv) {
    float distanceToFire = distance(uv, FIRE_LIGHT_POSITION);

    return 1.0 - smoothstep(
        FIRE_LIGHT_INNER_RADIUS,
        FIRE_LIGHT_OUTER_RADIUS,
        distanceToFire
    );
}

// Añade la iluminación procedente del fuego al color de la pared
vec3 applyFireLight(vec3 color, vec2 uv) {
    float glow = fireGlowMask(uv);

    color +=
        FIRE_LIGHT_COLOR
        * glow
        * FIRE_LIGHT_INTENSITY;

    return color;
}

void main() {
    vec2 uv = vUV;

    vec3 color = WALL_BASE_COLOR;
    color = applyWallLines(color, uv);
    color = applyFireLight(color, uv);

    FragColor = vec4(color, 1.0);
}