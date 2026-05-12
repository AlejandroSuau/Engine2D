#version 150 core

in vec3 aPos;
in vec2 aUV;

out vec2 vUV;

void main() {
    vUV = aUV;
    gl_Position = vec4(aPos, 1.0);
}