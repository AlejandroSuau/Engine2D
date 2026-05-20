#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uSceneTexture;
uniform float uTime;

// ============================================================
//  heat.frag refactorizado
// ------------------------------------------------------------
//  Este shader genera la distorsión visual del aire caliente.
//
//  La idea general es:
//    1. Leer la escena ya renderizada desde una textura.
//    2. Crear una máscara que define dónde hay calor.
//    3. Generar ondas animadas ascendentes.
//    4. Desplazar ligeramente las coordenadas de muestreo.
//    5. Leer la textura desplazada para simular refracción.
//
//  No simula físicamente el calor. Es un postprocesado visual
//  inspirado en la refracción producida por aire caliente.
// ============================================================


// ------------------------------------------------------------
// Parámetros artísticos principales
// ------------------------------------------------------------

const float FIRE_CENTER_X = 0.5;

const float HEAT_BOTTOM_WIDTH = 0.30;
const float HEAT_TOP_WIDTH    = 0.11;

const float HEAT_HORIZONTAL_SOFTNESS = 0.65;

const float HEAT_VERTICAL_START = 0.12;
const float HEAT_VERTICAL_FULL  = 0.30;
const float HEAT_VERTICAL_FADE_START = 0.90;
const float HEAT_VERTICAL_FADE_END   = 1.00;

const float WAVE_1_FREQUENCY_Y = 45.0;
const float WAVE_1_FREQUENCY_X = 9.0;
const float WAVE_1_SPEED       = 8.0;

const float WAVE_2_FREQUENCY_Y = 80.0;
const float WAVE_2_SPEED       = 13.0;

const float DISTORTION_X_WAVE_1 = 0.0030;
const float DISTORTION_X_WAVE_2 = 0.0015;

const float DISTORTION_Y_WAVE_1 = 0.0020;
const float DISTORTION_Y_WAVE_2 = 0.0025;

// ------------------------------------------------------------
// Máscara de zona caliente
// ------------------------------------------------------------

float heatWidthAtHeight(float y) {
    // El calor ocupa más anchura cerca de la base del fuego
    // y se estrecha progresivamente al subir.
    return mix(HEAT_BOTTOM_WIDTH, HEAT_TOP_WIDTH, y);
}

float horizontalHeatMask(vec2 uv) {
    float distanceToFireAxis = abs(uv.x - FIRE_CENTER_X);
    float heatWidth = heatWidthAtHeight(uv.y);

    return 1.0 - smoothstep(
        heatWidth * HEAT_HORIZONTAL_SOFTNESS,
        heatWidth,
        distanceToFireAxis
    );
}

float verticalHeatMask(vec2 uv) {
    // El efecto aparece suavemente sobre la base del fuego
    // y desaparece al llegar a la zona superior de la pantalla.
    float appearFromBase = smoothstep(
        HEAT_VERTICAL_START,
        HEAT_VERTICAL_FULL,
        uv.y
    );

    float fadeAtTop = 1.0 - smoothstep(
        HEAT_VERTICAL_FADE_START,
        HEAT_VERTICAL_FADE_END,
        uv.y
    );

    return appearFromBase * fadeAtTop;
}

float heatMask(vec2 uv) {
    return horizontalHeatMask(uv)
         * verticalHeatMask(uv);
}


// ------------------------------------------------------------
// Ondas de distorsión
// ------------------------------------------------------------

float heatWave1(vec2 uv) {
    return sin(
        uv.y * WAVE_1_FREQUENCY_Y
        - uTime * WAVE_1_SPEED
        + uv.x * WAVE_1_FREQUENCY_X
    );
}

float heatWave2(vec2 uv) {
    return sin(
        uv.y * WAVE_2_FREQUENCY_Y
        - uTime * WAVE_2_SPEED
    );
}

vec2 heatDistortion(vec2 uv) {
    float wave1 = heatWave1(uv);
    float wave2 = heatWave2(uv);

    float distortionX =
        wave1 * DISTORTION_X_WAVE_1 +
        wave2 * DISTORTION_X_WAVE_2;

    float distortionY =
        wave1 * DISTORTION_Y_WAVE_1 +
        wave2 * DISTORTION_Y_WAVE_2;

    return vec2(distortionX, distortionY);
}

// ------------------------------------------------------------
// Programa principal del fragment shader
// ------------------------------------------------------------

void main() {
    vec2 uv = vUV;

    float mask = heatMask(uv);
    vec2 distortion = heatDistortion(uv);

    vec2 sceneUV = uv + distortion * mask;
    vec3 sceneColor = texture(uSceneTexture, sceneUV).rgb;

    FragColor = vec4(sceneColor, 1.0);
}