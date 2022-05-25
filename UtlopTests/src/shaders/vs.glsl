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
#define SPOT_LIGHT      16

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
struct SpotLightStr{
	vec3 direction;
	vec3 color;
	float intensity;
	vec3 LightPos;
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
layout(location = SPOT_LIGHT) uniform SpotLightStr spotLight;
uniform int hasNormalMap;
uniform int l_type;
uniform mat4 mat2proj;
uniform mat4 lightProjection;
uniform int hasLightCmp;
uniform vec3 colorSilhoutte;

out vec2 text_coords;
out vec3 frag_position;
out vec3 frag_normal;
out vec3 FragPos; 
out vec3 camPosition;
out vec3 f_colorSilhoutte;
out flat int lightType;
out vec4 fragPosLight;
out int hasLightComponent;
flat out int doesHasNormalMap;
out PointLightStr pointLightSTR;
out DirectionalLightStr directionalLightSTR;
out SpotLightStr spotLightSTR;

void main()
{
  f_colorSilhoutte = colorSilhoutte;
	pointLightSTR = pointLight;
  directionalLightSTR = directionalLight;
  spotLightSTR = spotLight;
	lightType = l_type;
  hasLightComponent = hasLightCmp;
	doesHasNormalMap = hasNormalMap;
	camPosition = camPos;

	fragPosLight = lightProjection * ModelMatrix * vec4(position, 1.0f);
    text_coords = aTexCoord;
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0f);
	frag_position = vec3(ModelMatrix * vec4(position, 1.0));
	frag_normal = mat3(transpose(inverse(ModelMatrix))) * normalize(normals);
	//FragPos = ;
}