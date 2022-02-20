#version 460 core

layout(location = 0) out vec4 FragColor;

in vec2 text_coords;
layout(location = 3) uniform sampler2D u_texture;

void main()
{    
    FragColor = texture(u_texture, text_coords);
}