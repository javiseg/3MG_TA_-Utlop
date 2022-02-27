#version 460 core
#define VERT_POSITION   0
#define VERT_COLOR      1
#define MODEL_VIEW      2
#define VERT_TEXTURE    3
#define VERT_NORMAL     4


layout (location = VERT_POSITION) in vec3 position;
layout (location = VERT_COLOR) uniform vec3 new_color;
layout (location = MODEL_VIEW) uniform mat4 u_vp_matrix;
layout (location = VERT_TEXTURE) in vec2 aTexCoord;
layout (location = VERT_NORMAL) in vec3 normals;
out vec3 out_color;
out vec2 text_coords;

layout(location = 5) uniform mat4 ModelMatrix;
layout(location = 6) uniform mat4 ViewMatrix;
layout(location = 7) uniform mat4 ProjectionMatrix;

void main()
{
	out_color = new_color;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
}