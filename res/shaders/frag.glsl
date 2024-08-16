#version 330 core

in vec3 _Color;
in vec2 _TexCoord;

out vec4 Color;

uniform sampler2D _texture1;
uniform sampler2D _textur2e2;

uniform int texture_unit;

void main() {
    if (texture_unit == 0) {
        Color = texture(_texture1, _TexCoord);
    } else if (texture_unit == 1) {
        Color = texture(_textur2e2, _TexCoord);
    } else {
        Color = vec4(_Color, 1.0);
    }
    
}