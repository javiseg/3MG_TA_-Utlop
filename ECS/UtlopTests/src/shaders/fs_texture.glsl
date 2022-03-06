#version 460 core
layout(location = 0) out vec4 color;
in vec3 out_color;
in vec2 text_coords;
in vec3 out_normals;

uniform sampler2D ourTexture;

void main()
{
	color = texture(ourTexture, text_coords);
}