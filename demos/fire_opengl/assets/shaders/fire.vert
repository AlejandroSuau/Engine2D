#version 150 core

in vec3 aPos;
in vec2 aUV;

out vec2 vUV;

uniform float uOffsetX;
uniform float uScale;

void main() {
    vUV = aUV;

    vec3 pos = aPos;
    pos.x *= uScale;
    pos.y *= uScale;
    pos.x += uOffsetX;

    gl_Position = vec4(pos, 1.0);
}