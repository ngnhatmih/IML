#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in vec2 TexCoord;

out vec3 _Color;
out vec2 _TexCoord;

uniform float alpha;
uniform bool upside_down;
uniform float x_offset;
uniform float y_offset;

void main() {
    float len = sqrt( Pos.x * Pos.x + Pos.y * Pos.y ); 
    float angle = 3.141592 - atan(Pos.y, Pos.x) - alpha;
    vec2 transform_pos = len * vec2(cos(angle), sin(angle));
    if (!upside_down) {
        gl_Position = vec4(transform_pos.x + x_offset, transform_pos.y + y_offset, Pos.z , 1.0);
    } else {
        gl_Position = vec4(transform_pos.x + x_offset, -transform_pos.y + y_offset, Pos.z , 1.0);
    }
    
    _Color = in_Color;
    _TexCoord = TexCoord;
}