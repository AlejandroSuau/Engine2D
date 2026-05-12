#version 150 core

in vec3 aPos;
in vec2 aUV;

uniform float uOffsetX;
uniform float uOffsetY;
uniform float uScaleX;
uniform float uScaleY;

void main() {
    vec3 pos = aPos;

    pos.x *= uScaleX;
    pos.y *= uScaleY;

    pos.x += uOffsetX;
    pos.y += uOffsetY;

    gl_Position = vec4(pos, 1.0);
}