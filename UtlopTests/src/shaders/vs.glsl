#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) uniform vec3 new_color;
out vec3 out_color;

void main()
{
    gl_Position = vec4(position, 1.0);
	out_color = new_color;
}