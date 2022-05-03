#version 460 core
#define VERT_POSITION   0
#define VERT_TEXTURE    1
#define VERT_NORMAL     2
#define MODEL_MATRIX    3
#define VIEW_MATRIX     4
#define PROJ_MATRIX     5
#define DIR_LIGHT	    6

struct Directional{
	vec3 direction;
	vec3 color;
	float intensity;
};

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXTURE) in vec2 aTexCoord;
layout(location = VERT_NORMAL) in vec3 normals;
layout(location = MODEL_MATRIX) uniform mat4 ModelMatrix;
layout(location = VIEW_MATRIX) uniform mat4 ViewMatrix;
layout(location = PROJ_MATRIX) uniform mat4 ProjectionMatrix;

layout(location = DIR_LIGHT) uniform Directional dirLight;
layout(location = 9) uniform vec3 dirLightPos;


out vec2 text_coords;
out vec3 frag_position;
out vec3 frag_normal;
out vec3 dirLightDirection;
out vec3 dirLightColor;
out float dirLightIntensity;
out vec3 dirLightPosition;

void main()
{
	dirLightDirection = dirLight.direction;
	dirLightColor = dirLight.color;
	dirLightIntensity = dirLight.intensity;
	dirLightPosition = dirLightPos;

    text_coords = aTexCoord;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
	frag_position = position;
	frag_normal = normals;
}