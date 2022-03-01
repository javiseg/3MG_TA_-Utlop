#version 460 core
layout(location = 0) out vec4 color;
in vec3 out_color;
in vec2 text_coords;

uniform sampler2D ourTexture;

void main()
{
	color = vec4(out_color, 1.0f);
}
