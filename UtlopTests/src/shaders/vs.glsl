#version 460 core
#define VERT_POSITION   0
#define VERT_TEXTURE    1
#define VERT_NORMAL     2


layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXTURE) in vec2 aTexCoord;
layout(location = VERT_NORMAL) in vec3 normals;
layout(location = 3) uniform mat4 ModelMatrix;
layout(location = 4) uniform mat4 ViewMatrix;
layout(location = 5) uniform mat4 ProjectionMatrix;

out vec2 text_coords;

void main()
{
    text_coords = aTexCoord;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
}