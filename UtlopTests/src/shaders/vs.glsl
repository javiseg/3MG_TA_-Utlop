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

void main()
{
	text_coords = aTexCoord;
    gl_Position = u_vp_matrix * vec4(position, 1.0);
	out_color = new_color;
}