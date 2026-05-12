#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

void main() {
    vec2 uv = vUV;

    // Parpadeo en la base
    float t = uv.y;
    float baseFlicker = sin(uTime * 12.0 + uv.x * 20.0) * 0.02;
    uv.x += baseFlicker * (1.0 - t);

    // Movimiento lateral principal
    float sway =
        sin(uTime * 2.0 + uv.y * 7.0) * 0.035 +
        sin(uTime * 3.6 + uv.y * 15.0) * 0.020 +
        sin(uTime * 6.0 + uv.y * 28.0) * 0.008;

    uv.x += sway * (0.35 + uv.y * 1.15);

    // Ruido barato procedural
    float noise =
        sin(uv.y * 40.0 + uTime * 6.0) * 0.02 +
        sin(uv.y * 90.0 + uTime * 3.0) * 0.01 +
        sin((uv.x + uv.y) * 60.0 + uTime * 5.0) * 0.015;

    uv.x += noise * (0.5 + uv.y);

    float distX = abs(uv.x - 0.5);

    // Anchura base
    float width = mix(0.26, 0.015, t);

    // Ruptura orgánica
    width += sin(t * 12.0 + uTime * 4.5) * 0.012;
    width += sin(t * 27.0 + uTime * 2.7) * 0.007;
    width += sin((uv.x + t) * 18.0 + uTime * 5.5) * 0.006;

    // Lenguas irregulares
    width += sin(uv.x * 10.0 + uv.y * 25.0 + uTime * 4.0) * 0.01;

    // Más rotura arriba
    width -= smoothstep(0.55, 1.0, t) * 0.035;

    // Colores
    vec3 bottomColor = vec3(1.0, 0.95, 0.75);
    vec3 middleColor = vec3(1.0, 0.52, 0.10);
    vec3 topColor    = vec3(0.35, 0.02, 0.00);

    vec3 outerColor;
    if (t < 0.28) {
        float k = t / 0.28;
        outerColor = mix(bottomColor, middleColor, k);
    } else {
        float k = (t - 0.28) / 0.72;
        outerColor = mix(middleColor, topColor, k);
    }

    // Núcleo más cálido
    float innerWidth = width * mix(0.58, 0.22, t);
    float inner = 1.0 - smoothstep(innerWidth * 0.55, innerWidth, distX);
    inner *= (1.0 - smoothstep(0.72, 1.0, t));

    vec3 coreColor = vec3(1.0, 0.85, 0.55);
    vec3 core = coreColor * inner * 1.25;

    vec3 color = outerColor + core;

    // Halo exterior
    float glow = 1.0 - smoothstep(width, width + 0.12, distX);
    glow *= (1.0 - t) * 0.55;
    vec3 glowColor = vec3(1.0, 0.38, 0.08) * glow;

    // Alpha suave en bordes
    float alpha = 1.0 - smoothstep(width - 0.02, width, distX);

    // Desvanecer arriba
    alpha *= 1.0 - smoothstep(0.76, 1.0, t);

    // Parpadeo leve general
    alpha *= 0.94 + 0.06 * sin(uTime * 9.0 + t * 11.0);

    FragColor = vec4(color + glowColor, max(alpha, glow * 0.22));
}