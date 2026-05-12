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
    //x += sin(uTime * 2.5 + y * 8.0) * 0.04 * y;
    x += (n - 0.5) * 0.16 * y;
    x += sin(y * 18.0 + uTime * 7.0) * 0.025 * y;

    // ===============================
    // Forma tipo hoguera
    // ===============================

    // Ancha abajo, pero no termina tan fina arriba
    float baseWidth = mix(0.52, 0.12, y);

    // Varias lenguas de fuego
    float tongue1 = sin(y * 10.0 - uTime * 3.0) * 0.10;
    float tongue2 = sin(y * 15.0 + uTime * 4.0) * 0.07;
    float tongue3 = sin(y * 23.0 - uTime * 5.5) * 0.04;

    // Centro ondulante
    float centerOffset =
        tongue1 * 0.35 +
        tongue2 * 0.25 +
        tongue3 * 0.20;

    centerOffset *= y;

    // Tres centros de llama
    float dCenter = abs(x - centerOffset);
float dLeft   = abs(x + 0.15 + centerOffset * 0.35);
float dRight  = abs(x - 0.15 + centerOffset * 0.30);

    // Anchuras diferentes
float mainWidth  = baseWidth * 0.90;
float leftWidth  = baseWidth * 0.78;
float rightWidth = baseWidth * 0.75;

    // Alturas diferentes
    float mainFlame =
        1.0 - smoothstep(mainWidth * 0.55, mainWidth, dCenter);

    mainFlame *=
        1.0 - smoothstep(0.78, 1.0, y);

    float leftFlame =
        1.0 - smoothstep(leftWidth * 0.30, leftWidth, dLeft);

    leftFlame *=
        1.0 - smoothstep(0.52, 0.82, y);

    float rightFlame =
        1.0 - smoothstep(rightWidth * 0.30, rightWidth, dRight);

    rightFlame *=
        1.0 - smoothstep(0.45, 0.75, y);

    // Combinar lenguas
    float flame =
        max(mainFlame, max(leftFlame, rightFlame));

        // Une visualmente las lenguas para que no parezcan tres columnas separadas
float body =
    1.0 - smoothstep(baseWidth * 0.45, baseWidth * 1.05, abs(x));

body *= 1.0 - smoothstep(0.58, 0.92, y);

flame = max(flame, body * 0.65);

    // Nace suavemente desde abajo
    flame *= smoothstep(0.00, 0.06, y);

    // Ruido para romper silueta
    flame *= smoothstep(0.15, 0.95, n + flame * 0.45);

    // Para que siga habiendo una variable d compatible con el resto del shader
    float d = dCenter;
    float width = baseWidth;

    // Nace suavemente en la base
    flame *= smoothstep(0.02, 0.16, y);

    // Se apaga arriba
    flame *= 1.0 - smoothstep(0.75, 1.0, y);

    // Núcleo interior
// Núcleos interiores para cada lengua de fuego
float mainCoreWidth  = mainWidth  * 0.34;
float leftCoreWidth  = leftWidth  * 0.32;
float rightCoreWidth = rightWidth * 0.32;

float mainCore =
    1.0 - smoothstep(mainCoreWidth * 0.45, mainCoreWidth, dCenter);

mainCore *=
    1.0 - smoothstep(0.45, 0.82, y);

float leftCore =
    1.0 - smoothstep(leftCoreWidth * 0.45, leftCoreWidth, dLeft);

leftCore *=
    1.0 - smoothstep(0.24, 0.56, y);

float rightCore =
    1.0 - smoothstep(rightCoreWidth * 0.45, rightCoreWidth, dRight);

rightCore *=
    1.0 - smoothstep(0.22, 0.52, y);

float core =
    max(mainCore, max(leftCore, rightCore));

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

    // Recorte de seguridad para que no aparezcan trozos sueltos
    float safeArea =
        smoothstep(0.05, 0.14, uv.x) *
        (1.0 - smoothstep(0.86, 0.95, uv.x)) *
        smoothstep(0.02, 0.10, uv.y) *
        (1.0 - smoothstep(0.88, 0.98, uv.y));

    alpha *= safeArea;
    flame *= safeArea;
    glow *= safeArea;

    if (alpha < 0.03) {
        discard;
    }

    FragColor = vec4(color * flame + glowColor, alpha);
}