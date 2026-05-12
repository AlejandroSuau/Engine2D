#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x)
         + (c - a) * u.y * (1.0 - u.x)
         + (d - b) * u.x * u.y;
}

float fbm(vec2 p) {
    float v = 0.0;
    float a = 0.5;

    v += a * noise(p); p *= 2.0; a *= 0.5;
    v += a * noise(p); p *= 2.0; a *= 0.5;
    v += a * noise(p); p *= 2.0; a *= 0.5;
    v += a * noise(p);

    return v;
}

void main() {
    vec2 uv = vUV;

    float y = uv.y;
    float x = uv.x - 0.5;

    // Ruido que sube
    float n = fbm(vec2(uv.x * 3.0, uv.y * 4.0 - uTime * 2.0));

    // Movimiento lateral, más fuerte arriba
    x += sin(uTime * 2.5 + y * 8.0) * 0.04 * y;
    x += (n - 0.5) * 0.12 * y;

    // Ancha abajo, estrecha arriba
    float width = mix(0.34, 0.03, y);

    // Romper borde de la llama
    width += (n - 0.5) * 0.12;

    float d = abs(x);

    // Forma principal
    float flame = 1.0 - smoothstep(width * 0.65, width, d);

    // Nace suavemente en la base
    flame *= smoothstep(0.02, 0.16, y);

    // Se apaga arriba
    flame *= 1.0 - smoothstep(0.75, 1.0, y);

    // Núcleo interior
    float coreWidth = width * 0.38;
    float core = 1.0 - smoothstep(coreWidth * 0.5, coreWidth, d);
    core *= 1.0 - smoothstep(0.45, 0.85, y);

    // Color por altura
    vec3 colBase = vec3(1.0, 0.95, 0.55);
    vec3 colMid  = vec3(1.0, 0.35, 0.03);
    vec3 colTop  = vec3(0.28, 0.02, 0.00);

    vec3 color = mix(colBase, colMid, smoothstep(0.05, 0.45, y));
    color = mix(color, colTop, smoothstep(0.45, 0.95, y));

    color += core * vec3(1.0, 0.75, 0.30);

    // Glow bajo, sin crear barra horizontal
    float glow = 1.0 - smoothstep(width, width + 0.16, d);
    glow *= smoothstep(0.10, 0.35, y);
    glow *= 1.0 - smoothstep(0.65, 1.0, y);

    vec3 glowColor = vec3(1.0, 0.25, 0.03) * glow * 0.5;

    float alpha = flame + glow * 0.25;
    alpha *= 0.90 + 0.10 * sin(uTime * 10.0 + n * 8.0);

    FragColor = vec4(color * flame + glowColor, alpha);
}