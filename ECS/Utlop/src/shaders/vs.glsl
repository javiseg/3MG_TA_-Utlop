#version 460 core
layout (location = 0) in vec4 position;
uniform mat4 u_m_matrix;
uniform mat4 u_vp_matrix;
out vec4 pos_color;

void main()
{
    gl_Position = u_vp_matrix * u_m_matrix * position;
	pos_color = position;
}