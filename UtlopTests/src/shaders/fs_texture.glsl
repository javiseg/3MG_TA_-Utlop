#version 460 core
in vec2 text_coords;
in vec3 frag_position;
in vec3 frag_normal;
in vec3 FragPos;

in vec3 dirLightDirection;
in vec3 dirLightColor;
in float dirLightIntensity;
in vec3 dirLightPosition;

in vec3 camPosition;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
//vec3 lightColor;
//vec3 lightPos;

struct Directional{
	vec3 direction;
	vec3 color;
	float intensity;
	vec3 dirLightPos;
};

out vec4 gl_FragColor; 

void main()
{
	Directional n_dirLight;
	n_dirLight.direction = dirLightDirection;
	n_dirLight.color = dirLightColor;
	n_dirLight.intensity = dirLightIntensity;

	float ambient = 0.20f;
	
	n_dirLight.direction = normalize(dirLightPosition - frag_position);

	vec4 textures = texture(diffuse0, text_coords);

	vec3 FinalResult = textures.rgb;
	
	float diffuse = max(dot(n_dirLight.direction, frag_normal) * 1 /*n_dirLight.intensity*/, 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPosition - frag_position);
	vec3 reflectionDirection = reflect(-n_dirLight.direction, frag_normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;


	if(n_dirLight.intensity == 0){
		gl_FragColor = vec4(FinalResult,1.0f);
	}else{
	
		gl_FragColor = (texture(diffuse0, text_coords) * (diffuse + ambient) + texture(specular0, text_coords).r * specular) * vec4(n_dirLight.color,1.0f);;
	}
	
}
