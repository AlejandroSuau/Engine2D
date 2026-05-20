#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

// ============================================================
//  fire.frag refactorizado
// ------------------------------------------------------------
//  Este shader genera una llama procedural sobre un quad.
//
//  La idea general es:
//    1. Usar las coordenadas UV del quad.
//    2. Crear ruido animado que sube con el tiempo.
//    3. Deformar lateralmente la llama con ese ruido.
//    4. Construir varias "lenguas" de fuego con smoothstep().
//    5. Calcular un núcleo interior más brillante.
//    6. Asignar color según la altura.
//    7. Usar alpha para recortar la silueta de la llama.
//
//  No es una simulación física de fuego. Es un efecto visual
//  procedural ajustado artísticamente mediante parámetros.
// ============================================================

// ------------------------------------------------------------
// Parámetros artísticos principales
// ------------------------------------------------------------

const float FLAME_BOTTOM_WIDTH = 0.52;
const float FLAME_TOP_WIDTH    = 0.12;

const float NOISE_SCALE_X = 3.0;
const float NOISE_SCALE_Y = 4.0;
const float NOISE_SPEED   = 2.0;

const float NOISE_DISTORTION_AMOUNT = 0.16;
const float WAVE_DISTORTION_AMOUNT  = 0.025;
const float WAVE_FREQUENCY          = 18.0;
const float WAVE_SPEED              = 7.0;

const float MAIN_TONGUE_HEIGHT_START  = 0.78;
const float MAIN_TONGUE_HEIGHT_END    = 1.00;
const float LEFT_TONGUE_HEIGHT_START  = 0.52;
const float LEFT_TONGUE_HEIGHT_END    = 0.82;
const float RIGHT_TONGUE_HEIGHT_START = 0.45;
const float RIGHT_TONGUE_HEIGHT_END   = 0.75;

const float BASE_FADE_START = 0.02;
const float BASE_FADE_END   = 0.16;
const float TOP_FADE_START  = 0.75;
const float TOP_FADE_END    = 1.00;

// ------------------------------------------------------------
// Utilidades de ruido procedural
// ------------------------------------------------------------
// Estas funciones generan valores pseudoaleatorios suaves.
// El ruido se usa para que la llama no tenga bordes perfectos.

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float valueNoise(vec2 p) {
    vec2 cell = floor(p);
    vec2 local = fract(p);

    float bottomLeft  = hash(cell);
    float bottomRight = hash(cell + vec2(1.0, 0.0));
    float topLeft     = hash(cell + vec2(0.0, 1.0));
    float topRight    = hash(cell + vec2(1.0, 1.0));

    // Curva de suavizado tipo Hermite: evita saltos bruscos entre celdas.
    vec2 smoothLocal = local * local * (3.0 - 2.0 * local);

    float bottom = mix(bottomLeft, bottomRight, smoothLocal.x);
    float top    = mix(topLeft, topRight, smoothLocal.x);

    return mix(bottom, top, smoothLocal.y);
}

float fractalNoise(vec2 p) {
    // Varias capas de ruido: una grande y otras cada vez más pequeñas.
    // Esto da un aspecto más orgánico que usar una sola capa.
    float result = 0.0;
    result += 0.6 * valueNoise(p);
    result += 0.3 * valueNoise(p * 2.0);
    result += 0.1 * valueNoise(p * 4.0);
    return result;

    return result;
}

float animatedFireNoise(vec2 uv) {
    vec2 noiseCoordinates = vec2(
        uv.x * NOISE_SCALE_X,
        uv.y * NOISE_SCALE_Y - uTime * NOISE_SPEED
    );

    return fractalNoise(noiseCoordinates);
}

// ------------------------------------------------------------
// Deformación de la llama
// ------------------------------------------------------------

float flameWidthAtHeight(float y) {
    // Abajo la llama es ancha; arriba se estrecha.
    return mix(FLAME_BOTTOM_WIDTH, FLAME_TOP_WIDTH, y);
}

float flameCenterOffset(float y) {
    return sin(y * 10.0 - uTime * 3.0) * 0.04 * y;
}

float distortedX(vec2 uv, float noiseValue) {
    float y = uv.y;
    float x = uv.x - 0.5;

    // Deformación orgánica por ruido.
    x += (noiseValue - 0.5) * NOISE_DISTORTION_AMOUNT * y;

    // Ondulación periódica adicional.
    x += sin(y * WAVE_FREQUENCY + uTime * WAVE_SPEED)
       * WAVE_DISTORTION_AMOUNT
       * y;

    return x;
}

// ------------------------------------------------------------
// Máscaras de forma
// ------------------------------------------------------------

float tongueMask(float distanceToCenter, float width, float fadeStart, float fadeEnd, float y) {
    // smoothstep permite hacer un borde suave:
    //  - cerca del centro vale casi 1
    //  - fuera de la anchura indicada cae hacia 0
    float mask = 1.0 - smoothstep(width * 0.30, width, distanceToCenter);

    // Cada lengua desaparece progresivamente al llegar a su altura máxima.
    mask *= 1.0 - smoothstep(fadeStart, fadeEnd, y);

    return mask;
}

float flameBodyMask(float x, float baseWidth, float y) {
    // Cuerpo común que une visualmente las lenguas para que no parezcan
    // tres columnas independientes.
    float body = 1.0 - smoothstep(baseWidth * 0.45, baseWidth * 1.05, abs(x));
    body *= 1.0 - smoothstep(0.58, 0.92, y);

    return body * 0.65;
}

float flameMask(vec2 uv, float x, float noiseValue) {
    float y = uv.y;
    float baseWidth = flameWidthAtHeight(y);
    float center = flameCenterOffset(y);

    // Distancia horizontal a cada una de las lenguas de fuego.
    float mainDistance  = abs(x - center);
    float leftDistance  = abs(x + 0.15 + center * 0.35);
    float rightDistance = abs(x - 0.15 + center * 0.30);

    float mainWidth  = baseWidth * 0.90;
    float leftWidth  = baseWidth * 0.78;
    float rightWidth = baseWidth * 0.75;

    float mainTongue = tongueMask(
        mainDistance,
        mainWidth,
        MAIN_TONGUE_HEIGHT_START,
        MAIN_TONGUE_HEIGHT_END,
        y
    );

    float leftTongue = tongueMask(
        leftDistance,
        leftWidth,
        LEFT_TONGUE_HEIGHT_START,
        LEFT_TONGUE_HEIGHT_END,
        y
    );

    float rightTongue = tongueMask(
        rightDistance,
        rightWidth,
        RIGHT_TONGUE_HEIGHT_START,
        RIGHT_TONGUE_HEIGHT_END,
        y
    );

    float mask = max(mainTongue, max(leftTongue, rightTongue));
    mask = max(mask, flameBodyMask(x, baseWidth, y));

    // La llama aparece suavemente desde la base.
    mask *= smoothstep(BASE_FADE_START, BASE_FADE_END, y);

    // Se apaga al llegar a la parte superior.
    mask *= 1.0 - smoothstep(TOP_FADE_START, TOP_FADE_END, y);

    // El ruido rompe la silueta para evitar una forma demasiado geométrica.
    mask *= smoothstep(0.15, 0.95, noiseValue + mask * 0.45);

    return mask;
}

float coreMask(vec2 uv, float x) {
    float y = uv.y;
    float baseWidth = flameWidthAtHeight(y);
    float center = flameCenterOffset(y);

    float mainDistance  = abs(x - center);
    float leftDistance  = abs(x + 0.15 + center * 0.35);
    float rightDistance = abs(x - 0.15 + center * 0.30);

    float mainCoreWidth  = baseWidth * 0.90 * 0.34;
    float leftCoreWidth  = baseWidth * 0.78 * 0.32;
    float rightCoreWidth = baseWidth * 0.75 * 0.32;

    float mainCore = 1.0 - smoothstep(mainCoreWidth * 0.45, mainCoreWidth, mainDistance);
    mainCore *= 1.0 - smoothstep(0.45, 0.82, y);

    float leftCore = 1.0 - smoothstep(leftCoreWidth * 0.45, leftCoreWidth, leftDistance);
    leftCore *= 1.0 - smoothstep(0.24, 0.56, y);

    float rightCore = 1.0 - smoothstep(rightCoreWidth * 0.45, rightCoreWidth, rightDistance);
    rightCore *= 1.0 - smoothstep(0.22, 0.52, y);

    return max(mainCore, max(leftCore, rightCore));
}

// ------------------------------------------------------------
// Color
// ------------------------------------------------------------

vec3 fireColor(float y, float core) {
    vec3 baseColor = vec3(1.0, 0.95, 0.55); // amarillo claro
    vec3 midColor  = vec3(1.0, 0.35, 0.03); // naranja intenso
    vec3 topColor  = vec3(0.28, 0.02, 0.00); // rojo oscuro

    vec3 color = mix(baseColor, midColor, smoothstep(0.05, 0.45, y));
    color = mix(color, topColor, smoothstep(0.45, 0.95, y));

    // El núcleo interior añade brillo cálido.
    color += core * vec3(1.0, 0.75, 0.30);

    return color;
}

// ------------------------------------------------------------
// Programa principal del fragment shader
// ------------------------------------------------------------

void main() {
    vec2 uv = vUV;

    float noiseValue = animatedFireNoise(uv);
    float x = distortedX(uv, noiseValue);

    float flame = flameMask(uv, x, noiseValue);
    float core  = coreMask(uv, x);
    vec3 color = fireColor(uv.y, core);

    FragColor = vec4(color * flame, flame);
}
