#version 150 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;
uniform float uIntensity;
uniform float uWind;
uniform float uSeed;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
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
    float value = 0.0;
    float amplitude = 0.5;

    value += amplitude * noise(p);
    p *= 2.0;
    amplitude *= 0.5;

    value += amplitude * noise(p);
    p *= 2.0;
    amplitude *= 0.5;

    value += amplitude * noise(p);
    p *= 2.0;
    amplitude *= 0.5;

    value += amplitude * noise(p);

    return value;
}

void main() {
    vec2 uv = vUV;

    float height = uv.y;

    // Movimiento del fuego
    uv.x += sin(uTime * 3.0 + uv.y * 8.0 + uSeed) * 0.04;
    uv.x += uWind * 0.05 * height;

    // El ruido sube con el tiempo
    vec2 noiseUV = vec2(
        uv.x * 3.0,
        uv.y * 4.0 - uTime * 1.8
    );

    float n = fbm(noiseUV);

    // Centro de la llama
    float center = 0.5;

    // La llama es ancha abajo y estrecha arriba
    float width = mix(0.32, 0.03, height);

    // El ruido rompe la silueta
    width += (n - 0.5) * 0.16;

    float dist = abs(uv.x - center);

    // Máscara principal de la llama
    float flame = 1.0 - smoothstep(width * 0.65, width, dist);

    // Se apaga arriba
    flame *= 1.0 - smoothstep(0.75, 1.0, height);

    // Más fuerte abajo
    flame *= smoothstep(0.0, 0.15, height);

    // Núcleo blanco/amarillo
    float coreWidth = width * 0.38;
    float core = 1.0 - smoothstep(coreWidth * 0.5, coreWidth, dist);
    core *= 1.0 - smoothstep(0.55, 0.95, height);

    // Colores por altura
    vec3 bottomColor = vec3(1.0, 0.95, 0.65);
    vec3 midColor    = vec3(1.0, 0.38, 0.04);
    vec3 topColor    = vec3(0.35, 0.02, 0.00);

    vec3 fireColor;

    if (height < 0.35) {
        fireColor = mix(bottomColor, midColor, height / 0.35);
    } else {
        fireColor = mix(midColor, topColor, (height - 0.35) / 0.65);
    }

    // Añadimos núcleo caliente
    fireColor += core * vec3(1.0, 0.75, 0.35);

    // Parpadeo general
    float flicker = 0.85 + 0.15 * sin(uTime * 12.0 + n * 6.0);

    fireColor *= flicker * uIntensity;

    // Halo exterior
    float glow = 1.0 - smoothstep(width, width + 0.18, dist);

    // Evita la franja horizontal en la base
    glow *= smoothstep(0.08, 0.22, height);

    // El glow se apaga arriba
    glow *= 1.0 - height;
    vec3 glowColor = vec3(1.0, 0.28, 0.03) * glow * 0.65;

    float alpha = flame;
    alpha += glow * 0.18;

    // Fade general en la base para que no se vea el borde del quad
    alpha *= smoothstep(0.03, 0.16, height);

    alpha *= uIntensity;

    FragColor = vec4(fireColor * flame + glowColor, alpha);
}