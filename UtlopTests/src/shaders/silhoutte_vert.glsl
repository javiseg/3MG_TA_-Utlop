#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoords;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec3 color;

out vec3 f_color;
out vec2 texCoords;
void main()
{
  texCoords = texcoords;
  f_color = color;
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
}