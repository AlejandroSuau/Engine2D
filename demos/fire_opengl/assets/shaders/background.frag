#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

float hash(vec2 p) {
    return fract(
        sin(dot(p, vec2(127.1, 311.7))) *
        43758.5453
    );
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u =
        f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x)
         + (c - a) * u.y * (1.0 - u.x)
         + (d - b) * u.x * u.y;
}

void main() {
    vec2 uv = vUV;

    // Color base de pared
    vec3 wall =
        vec3(0.045, 0.038, 0.035);

    // Ruido suave para que no parezca plano
    float n =
        noise(uv * 18.0);

    wall +=
        (n - 0.5) * 0.035;

    // Líneas horizontales de pared/ladrillos
    float lineWave = sin(uv.y * 55.0);

    float lineMask =
        smoothstep(0.91, 1.0, lineWave);

    wall +=
        lineMask * vec3(0.09, 0.055, 0.032);

    // Pequeñas divisiones verticales muy suaves
    float verticalLines = sin(uv.x * 22.0);

    float verticalMask =
        smoothstep(0.996, 1.0, verticalLines);

    wall +=
        verticalMask * vec3(0.015, 0.010, 0.008);

    // Posición aproximada del fuego
    vec2 firePos = vec2(0.5, 0.32);

    float dist =
        distance(uv, firePos);

    // Luz naranja del fuego sobre la pared
    float glow =
        1.0 - smoothstep(0.04, 0.62, dist);

    float flicker =
        0.85 + 0.15 * sin(uTime * 8.0);

    vec3 fireLight =
        vec3(1.0, 0.38, 0.07);

    wall +=
        fireLight * glow * 0.45 * flicker;

    // Base caliente / brasas
    vec2 emberPos =
        vec2(0.5, 0.12);

    vec2 emberUV = uv - emberPos;

    emberUV.y *= 2.5;

    float ember =
        1.0 - smoothstep(
            0.02,
            0.20,
            length(emberUV)
        );

    wall +=
        vec3(1.0, 0.22, 0.04) * ember * 0.35;

    // Oscurecimiento hacia los bordes
    float vignette =
        distance(uv, vec2(0.5, 0.5));

    wall *=
        1.0 - vignette * 0.65;

    FragColor =
        vec4(wall, 1.0);
}