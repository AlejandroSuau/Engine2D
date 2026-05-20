#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

// ============================================================
//  background.frag refactorizado
// ------------------------------------------------------------
//  Este shader genera un fondo procedural oscuro iluminado
//  por el fuego.
//
//  La idea general es:
//    1. Crear una pared oscura como color base.
//    2. Añadir ruido suave para evitar un color plano.
//    3. Añadir líneas horizontales y verticales como textura.
//    4. Simular luz naranja del fuego sobre la pared.
//    5. Añadir una zona inferior cálida tipo brasas.
//    6. Oscurecer los bordes con una viñeta.
//
//  No representa una pared físicamente simulada.
//  Es un fondo procedural estilizado que sirve para reforzar
//  visualmente la llama y la distorsión térmica.
// ============================================================


// ------------------------------------------------------------
// Parámetros artísticos principales
// ------------------------------------------------------------

const vec3 WALL_BASE_COLOR = vec3(0.045, 0.038, 0.035);

const float WALL_NOISE_SCALE = 18.0;
const float WALL_NOISE_INTENSITY = 0.035;

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

const float FLICKER_BASE = 0.85;
const float FLICKER_AMOUNT = 0.15;
const float FLICKER_SPEED = 8.0;

const vec2 EMBER_POSITION = vec2(0.5, 0.12);
const float EMBER_VERTICAL_STRETCH = 2.5;
const float EMBER_INNER_RADIUS = 0.02;
const float EMBER_OUTER_RADIUS = 0.20;
const vec3 EMBER_COLOR = vec3(1.0, 0.22, 0.04);
const float EMBER_INTENSITY = 0.35;

const vec2 SCREEN_CENTER = vec2(0.5, 0.5);
const float VIGNETTE_STRENGTH = 0.65;


// ------------------------------------------------------------
// Ruido procedural
// ------------------------------------------------------------

float hash(vec2 p) {
    return fract(
        sin(dot(p, vec2(127.1, 311.7))) *
        43758.5453
    );
}

float valueNoise(vec2 p) {
    vec2 cell = floor(p);
    vec2 local = fract(p);

    float bottomLeft  = hash(cell);
    float bottomRight = hash(cell + vec2(1.0, 0.0));
    float topLeft     = hash(cell + vec2(0.0, 1.0));
    float topRight    = hash(cell + vec2(1.0, 1.0));

    vec2 smoothLocal =
        local * local * (3.0 - 2.0 * local);

    float bottom =
        mix(bottomLeft, bottomRight, smoothLocal.x);

    float top =
        mix(topLeft, topRight, smoothLocal.x);

    return mix(bottom, top, smoothLocal.y);
}


// ------------------------------------------------------------
// Textura de pared
// ------------------------------------------------------------

vec3 baseWallColor() {
    return WALL_BASE_COLOR;
}

float wallNoise(vec2 uv) {
    return valueNoise(uv * WALL_NOISE_SCALE);
}

vec3 applyWallNoise(vec3 color, vec2 uv) {
    float noiseValue = wallNoise(uv);

    color +=
        (noiseValue - 0.5) * WALL_NOISE_INTENSITY;

    return color;
}

float horizontalLineMask(vec2 uv) {
    float wave =
        sin(uv.y * HORIZONTAL_LINE_FREQUENCY);

    return smoothstep(
        HORIZONTAL_LINE_START,
        HORIZONTAL_LINE_END,
        wave
    );
}

float verticalLineMask(vec2 uv) {
    float wave =
        sin(uv.x * VERTICAL_LINE_FREQUENCY);

    return smoothstep(
        VERTICAL_LINE_START,
        VERTICAL_LINE_END,
        wave
    );
}

vec3 applyWallLines(vec3 color, vec2 uv) {
    float horizontalLines = horizontalLineMask(uv);
    float verticalLines = verticalLineMask(uv);

    color += horizontalLines * HORIZONTAL_LINE_COLOR;
    color += verticalLines * VERTICAL_LINE_COLOR;

    return color;
}


// ------------------------------------------------------------
// Iluminación procedente del fuego
// ------------------------------------------------------------

float fireGlowMask(vec2 uv) {
    float distanceToFire =
        distance(uv, FIRE_LIGHT_POSITION);

    return 1.0 - smoothstep(
        FIRE_LIGHT_INNER_RADIUS,
        FIRE_LIGHT_OUTER_RADIUS,
        distanceToFire
    );
}

float fireFlicker() {
    return FLICKER_BASE
         + FLICKER_AMOUNT * sin(uTime * FLICKER_SPEED);
}

vec3 applyFireLight(vec3 color, vec2 uv) {
    float glow = fireGlowMask(uv);
    float flicker = fireFlicker();

    color +=
        FIRE_LIGHT_COLOR
        * glow
        * FIRE_LIGHT_INTENSITY
        * flicker;

    return color;
}


// ------------------------------------------------------------
// Brasas / base caliente
// ------------------------------------------------------------

float emberMask(vec2 uv) {
    vec2 emberUV =
        uv - EMBER_POSITION;

    emberUV.y *= EMBER_VERTICAL_STRETCH;

    return 1.0 - smoothstep(
        EMBER_INNER_RADIUS,
        EMBER_OUTER_RADIUS,
        length(emberUV)
    );
}

vec3 applyEmberGlow(vec3 color, vec2 uv) {
    float ember = emberMask(uv);

    color +=
        EMBER_COLOR
        * ember
        * EMBER_INTENSITY;

    return color;
}


// ------------------------------------------------------------
// Oscurecimiento de bordes
// ------------------------------------------------------------

float vignetteMask(vec2 uv) {
    float distanceToCenter =
        distance(uv, SCREEN_CENTER);

    return 1.0 - distanceToCenter * VIGNETTE_STRENGTH;
}

vec3 applyVignette(vec3 color, vec2 uv) {
    return color * vignetteMask(uv);
}


// ------------------------------------------------------------
// Programa principal del fragment shader
// ------------------------------------------------------------

void main() {
    vec2 uv = vUV;

    vec3 color = baseWallColor();

    color = applyWallNoise(color, uv);
    color = applyWallLines(color, uv);
    color = applyFireLight(color, uv);
    color = applyEmberGlow(color, uv);
    color = applyVignette(color, uv);

    FragColor = vec4(color, 1.0);
}