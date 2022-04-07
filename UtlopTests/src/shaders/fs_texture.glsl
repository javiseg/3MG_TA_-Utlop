#version 460 core
in vec2 text_coords;

layout(binding = 0) uniform sampler2D ourTexture;

void main()
{
	gl_FragColor = texture(ourTexture, text_coords);
}
