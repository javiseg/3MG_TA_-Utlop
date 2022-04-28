#version 460 core
in vec2 text_coords;

layout(binding = 0) uniform sampler2D ourTexture;
vec3 lightColor;
vec3 lightPos;
void main()
{
	lightPos = vec3(1.2f, 1.0f, 2.0f);
	lightColor = vec3(1.0f,1.0f,1.0f);
	vec4 textures = texture(ourTexture, text_coords);
	gl_FragColor = textures;
}
