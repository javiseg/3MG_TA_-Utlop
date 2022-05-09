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
uniform sampler2D normal0;
//vec3 lightColor;
//vec3 lightPos;

struct Directional{
	vec3 direction;
	vec3 color;
	float intensity;
	vec3 dirLightPos;
};

out vec4 gl_FragColor; 


vec4 PointLight(){

	vec3 lightVec = dirLightPosition - frag_position;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 0.20f;
	float b = 0.70f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);


	float ambient = 0.20f;
	vec3 normal = normalize(frag_normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal,lightDirection), 0.0f);
	
	float specular = 0.0f;
	if(diffuse != 0){
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPosition - frag_position);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		vec3 reflectionDirection = reflect(-lightDirection, frag_normal);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	}

	return (texture(diffuse0, text_coords) * (diffuse * inten + ambient) + texture(specular0, text_coords).r * specular * inten) * vec4(dirLightColor,1.0f);
}



void main()
{

	
	
	if(dirLightIntensity == 0){
		vec4 textures = texture(diffuse0, text_coords);

		vec3 FinalResult = textures.rgb;
		gl_FragColor = vec4(FinalResult,1.0f);
	}else{
	
		gl_FragColor = PointLight();
	}
	
}
