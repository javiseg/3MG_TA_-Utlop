#version 460 core
#define VERT_POSITION   0
#define VERT_TEXTURE    1
#define VERT_NORMAL     2
#define MODEL_MATRIX    3
#define VIEW_MATRIX     4
#define PROJ_MATRIX     5
#define POINT_LIGHT	    6
#define CAM_POS			    11
#define DIR_LIGHT       12

struct DirectionalLightStr{
	vec3 direction;
	vec3 color;
	float intensity;
	vec3 dirLightPos;
};
struct PointLightStr{
	vec3 direction;
	vec3 color;
	float intensity;
	vec3 dirLightPos;
};

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXTURE) in vec2 aTexCoord;
layout(location = VERT_NORMAL) in vec3 normals;
layout(location = MODEL_MATRIX) uniform mat4 ModelMatrix;
layout(location = VIEW_MATRIX) uniform mat4 ViewMatrix;
layout(location = PROJ_MATRIX) uniform mat4 ProjectionMatrix;

layout(location = POINT_LIGHT) uniform PointLightStr pointLight;
layout(location = CAM_POS) uniform vec3 camPos;
layout(location = DIR_LIGHT) uniform DirectionalLightStr directionalLight;
uniform int hasNormalMap;
uniform int l_type;
uniform mat4 mat2proj;
uniform mat4 lightProjection;

out vec2 text_coords;
out vec3 frag_position;
out vec3 frag_normal;
out vec3 FragPos; 
out vec3 camPosition;
out flat int lightType;
out vec4 fragPosLight;
flat out int doesHasNormalMap;
out PointLightStr pointLightSTR;
out DirectionalLightStr directionalLightSTR;

void main()
{
	pointLightSTR = pointLight;
  directionalLightSTR = directionalLight;
	lightType = l_type;
	doesHasNormalMap = hasNormalMap;
	camPosition = camPos;

	fragPosLight = lightProjection * ModelMatrix * vec4(position, 1.0f);
    text_coords = aTexCoord;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
	frag_position = vec3(ModelMatrix * vec4(position, 1.0));
	frag_normal = mat3(transpose(inverse(ModelMatrix))) * normalize(normals);
	//FragPos = ;
}