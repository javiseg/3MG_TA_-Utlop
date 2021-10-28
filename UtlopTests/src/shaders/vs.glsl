#version 460 core
layout (location = 0) in vec4 position;
out vec4 pos_color;

void main()
{
    gl_Position = position;
	pos_color = position;
  pos_color.y *= -1.0f;
}