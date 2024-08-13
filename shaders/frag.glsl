#version 330 core

in vec3 _Color;
out vec4 Color;

void main() {
    Color = vec4(_Color, 1.0);
};