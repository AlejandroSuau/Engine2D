#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uSceneTexture;
uniform float uTime;

// Variables "artísticas" para evitar números mágicos en el código
const float FIRE_CENTER_X = 0.5;

const float HEAT_BOTTOM_WIDTH = 0.30;
const float HEAT_TOP_WIDTH = 0.11;

const float HEAT_HORIZONTAL_SOFTNESS = 0.65;

const float HEAT_VERTICAL_START = 0.12;
const float HEAT_VERTICAL_FULL = 0.30;
const float HEAT_VERTICAL_FADE_START = 0.90;
const float HEAT_VERTICAL_FADE_END = 1.00;

const float WAVE_1_FREQUENCY_Y = 45.0;
const float WAVE_1_FREQUENCY_X = 9.0;
const float WAVE_1_SPEED = 8.0;

const float WAVE_2_FREQUENCY_Y = 80.0;
const float WAVE_2_SPEED = 13.0;

const float DISTORTION_X_WAVE_1 = 0.0030;
const float DISTORTION_X_WAVE_2 = 0.0015;

const float DISTORTION_Y_WAVE_1 = 0.0020;
const float DISTORTION_Y_WAVE_2 = 0.0025;

// Calcula la anchura de la zona de calor según la altura.
float heatWidthAtHeight(float y) {
    return mix(HEAT_BOTTOM_WIDTH, HEAT_TOP_WIDTH, y);
}

// Máscara horizontal
// Mayor intensidad en el eje central
float horizontalHeatMask(vec2 uv) {
    float distanceToFireAxis = abs(uv.x - FIRE_CENTER_X);
    float heatWidth = heatWidthAtHeight(uv.y);

    return 1.0 - smoothstep(
        heatWidth * HEAT_HORIZONTAL_SOFTNESS,
        heatWidth,
        distanceToFireAxis
    );
}

// Máscara vertical
// Evita que el efecto aparezca de golpe en la base o que llegue con fuerza arriba
float verticalHeatMask(vec2 uv) {
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

// Primera onda
// Vibración lenta y ancha
float heatWave1(vec2 uv) {
    return sin(
        uv.y * WAVE_1_FREQUENCY_Y
        - uTime * WAVE_1_SPEED
        + uv.x * WAVE_1_FREQUENCY_X
    );
}

// Segunda onda
// Vibración más rápida y estrecha
float heatWave2(vec2 uv) {
    return sin(
        uv.y * WAVE_2_FREQUENCY_Y
        - uTime * WAVE_2_SPEED
    );
}

// Calcula el desplazamiento UV producido por el calor
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

void main() {
    vec2 uv = vUV;

    float mask = heatMask(uv);
    vec2 distortion = heatDistortion(uv);

    vec2 sceneUV = uv + distortion * mask;
    vec3 sceneColor = texture(uSceneTexture, sceneUV).rgb;

    FragColor = vec4(sceneColor, 1.0);
}