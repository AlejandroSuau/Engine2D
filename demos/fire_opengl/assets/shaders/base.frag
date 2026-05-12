#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

void main() {
    vec2 uv = vUV;

    // Gradiente de fondo: más oscuro arriba, más cálido abajo
    vec3 topColor = vec3(0.05, 0.05, 0.07);
    vec3 bottomColor = vec3(0.28, 0.16, 0.08);
    vec3 color = mix(bottomColor, topColor, uv.y);

    // Líneas horizontales para que la distorsión de calor se vea
    float lines = sin(uv.y * 80.0);
    float lineMask = smoothstep(0.92, 1.0, lines);

    color += lineMask * vec3(0.18, 0.12, 0.06);

    // Algunas líneas verticales suaves, tipo pared/suelo
    float vertical = sin(uv.x * 35.0);
    float verticalMask = smoothstep(0.96, 1.0, vertical);

    color += verticalMask * vec3(0.06, 0.04, 0.03);

    // Oscurecer bordes
    float dist = distance(uv, vec2(0.5, 0.5));
    color *= 1.0 - dist * 0.45;

    FragColor = vec4(color, 1.0);
}