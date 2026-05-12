#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uSceneTexture;
uniform float uTime;
uniform vec2 uResolution;

void main() {
    vec2 uv = vUV;

    // Centro del fuego en pantalla
    vec2 fireCenter = vec2(0.5, 0.22);

    // Distancia al eje vertical del fuego
    float distX = abs(uv.x - fireCenter.x);

    // El calor es más ancho abajo y más fino arriba
    float heatWidth = mix(0.30, 0.11, uv.y);

    float horizontalMask =
        1.0 - smoothstep(heatWidth * 0.65, heatWidth, distX);

    // Solo queremos calor por encima de la base
    float verticalMask =
        smoothstep(0.12, 0.30, uv.y) *
        (1.0 - smoothstep(0.90, 1.0, uv.y));

    // Más fuerte cerca del fuego
    float distanceToFire = distance(uv, fireCenter);

    float radialMask =
        1.0 - smoothstep(0.05, 0.65, distanceToFire);

    float mask =
        horizontalMask * verticalMask * radialMask;

    // Ondas ascendentes
    float wave1 =
        sin(uv.y * 45.0 - uTime * 8.0 + uv.x * 9.0);

    float wave2 =
        sin(uv.y * 80.0 - uTime * 13.0);

    float wave3 =
        sin((uv.x + uv.y) * 35.0 + uTime * 5.0);

float distortionX =
    wave1 * 0.003 +
    wave2 * 0.0015 +
    wave3 * 0.001;

float distortionY =
    wave1 * 0.002 +
    wave2 * 0.0025;

    vec2 screenUV =
        gl_FragCoord.xy / uResolution;

screenUV.x += distortionX * mask;
screenUV.y += distortionY * mask;

    screenUV =
        clamp(screenUV, vec2(0.001), vec2(0.999));

    vec3 sceneColor =
        texture(uSceneTexture, screenUV).rgb;

    FragColor =
        vec4(sceneColor, 1.0);
}