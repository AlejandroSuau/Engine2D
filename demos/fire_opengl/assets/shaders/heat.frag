#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uSceneTexture;
uniform float uTime;
uniform vec2 uResolution;

void main() {
    vec2 uv = vUV;

    float distX = abs(uv.x - 0.5);

    // El calor ocupa más zona que la llama
    float heatWidth = mix(0.34, 0.10, uv.y);

    float mask = 1.0 - smoothstep(heatWidth - 0.035, heatWidth, distX);

    // Que se note más en la zona media-alta
    float verticalFactor =
        smoothstep(0.08, 0.22, uv.y) *
        (1.0 - smoothstep(0.93, 1.0, uv.y));

    float turbulence1 = sin(uTime * 2.8 + uv.y * 16.0) * 0.010;
    float turbulence2 = sin(uTime * 5.2 + uv.y * 31.0) * 0.006;
    float turbulence3 = sin(uTime * 7.0 + uv.y * 52.0) * 0.003;

    float offsetX = (turbulence1 + turbulence2 + turbulence3) * mask * verticalFactor;
    float offsetY = (turbulence2 * 0.35) * mask * verticalFactor;

    vec2 screenUV = gl_FragCoord.xy / uResolution;
    screenUV += vec2(offsetX, offsetY);

    // Evita glitches de borde
    screenUV = clamp(screenUV, vec2(0.001), vec2(0.999));

    vec4 sceneColor = texture(uSceneTexture, screenUV);

    float alpha = mask * verticalFactor * 0.42;

    FragColor = vec4(sceneColor.rgb, alpha);
}