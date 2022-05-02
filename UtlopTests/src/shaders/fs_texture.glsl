#version 460 core
in vec2 text_coords;
in vec3 frag_position;
in vec3 frag_normal;

layout(binding = 0) uniform sampler2D ourTexture;
//vec3 lightColor;
//vec3 lightPos;

struct Directional{

	vec3 direction;
	vec3 color;
	float intensity;
};



out vec4 gl_FragColor; 

void main()
{
	vec3 lightPos = vec3(0.0f, 0.0f, 10.0f);
	//lightColor = vec3(1.0f,1.0f,1.0f);
	vec4 result = vec4(0.1);
	//float texture_specular = texture(ourTexture, text_coords).r;
	Directional directional_light;
	directional_light.color = vec3(1.0, 0.0f, 0.0f);
	directional_light.direction = normalize(lightPos - frag_position);

	directional_light.intensity = 1.1f;



	vec4 textures = texture(ourTexture, text_coords);

	vec3 FinalResult = textures.rgb;
	
	float brightness = dot(directional_light.direction, frag_normal) * directional_light.intensity;

	result += vec4(brightness);

	gl_FragColor = vec4(FinalResult,1.0f) * (result * vec4(directional_light.color, 1.0f));
}
