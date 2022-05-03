#version 460 core
in vec2 text_coords;
in vec3 frag_position;
in vec3 frag_normal;

in vec3 dirLightDirection;
in vec3 dirLightColor;
in float dirLightIntensity;
in vec3 dirLightPosition;

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
	Directional n_dirLight;
	n_dirLight.direction = dirLightDirection;
	n_dirLight.color = dirLightColor;
	n_dirLight.intensity = dirLightIntensity;


	vec3 lightPos = vec3(0.0f, 0.0f, 10.0f);
	//lightColor = vec3(1.0f,1.0f,1.0f);
	vec4 result = vec4(0.1);
	//float texture_specular = texture(ourTexture, text_coords).r;
	
	n_dirLight.direction = normalize(dirLightPosition - frag_position);




	vec4 textures = texture(ourTexture, text_coords);

	vec3 FinalResult = textures.rgb;
	
	float brightness = dot(n_dirLight.direction, frag_normal) * n_dirLight.intensity;

	result += vec4(brightness);

	if(n_dirLight.intensity == 0){
		gl_FragColor = vec4(FinalResult,1.0f);
	}else{
		gl_FragColor = vec4(FinalResult,1.0f) * (result * vec4(n_dirLight.color, 1.0f));
	}
	
}
