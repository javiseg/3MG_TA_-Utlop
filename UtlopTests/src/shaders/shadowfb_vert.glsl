#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 lightprojection;
uniform mat4 ModelMatrix;

void main()
{
    gl_Position = lightprojection * ModelMatrix * vec4(position, 1.0);
}