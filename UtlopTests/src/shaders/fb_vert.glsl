#version 460 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

uniform int postProcessType;

out vec2 texCoords;
out flat int f_postProcessType;

void main()
{
	texCoords = inTexCoords;
  f_postProcessType = postProcessType;
  gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0f);
}