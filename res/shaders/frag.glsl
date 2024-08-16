#version 330 core

in vec3 _Color;
in vec2 _TexCoord;

out vec4 Color;

uniform sampler2D _texture;

void main() {
    Color = texture(_texture, _TexCoord) * vec4(_Color, 1.0);
};