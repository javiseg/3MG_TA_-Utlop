#version 460 core
in vec2 text_coords;
in vec3 frag_position;
in vec3 frag_normal;
in vec3 FragPos;
in  flat int lightType;

flat in int doesHasNormalMap;
in vec3 camPosition;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;


struct PointLightStr{
	vec3 direction;
	vec3 color;
	float intensity;
	vec3 dirLightPos;
};

in PointLightStr pointLightSTR;

out vec4 gl_FragColor; 


vec4 PointLight(){

	vec3 lightVec = pointLightSTR.dirLightPos - frag_position;

	float dist = length(lightVec);
	float a = 0.005f;
	float b = 0.001f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
	vec3 normal = vec3(0.0f);

	normal = normalize(frag_normal);

	float ambient = 0.20f;
	
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal,lightDirection), 0.0f);
	
	float specular = 0.0f;
	if(diffuse != 0){
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPosition - frag_position);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		vec3 reflectionDirection = reflect(lightDirection, frag_normal);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	}

	return (texture(diffuse0, text_coords) * (diffuse * inten + ambient) + texture(specular0, text_coords).r * specular * inten) * vec4(pointLightSTR.color,1.0f);
}

vec4 DirectionalLight(){


	vec3 normal = normalize(frag_normal);

	float ambient = 0.20f;
	
	vec3 lightDirection = normalize(pointLightSTR.direction);
		
	float diffuse = max(dot(normal,lightDirection), 0.0f);
	
	float specular = 0.0f;
	if(diffuse != 0){
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPosition - frag_position);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		vec3 reflectionDirection = reflect(lightDirection, frag_normal);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	}

	return (texture(diffuse0, text_coords) * (diffuse + ambient) + texture(specular0, text_coords).r * specular) * vec4(pointLightSTR.color,1.0f);
}



void main()
{

	if(pointLightSTR.intensity == 0){
		vec4 textures = texture(diffuse0, text_coords);
		vec3 FinalResult = textures.rgb;
		gl_FragColor = vec4(FinalResult,1.0f);
	}else{
		if(lightType == 0){
			gl_FragColor = PointLight();
		}else if(lightType == 1){
			gl_FragColor = DirectionalLight();
		}
	}
	
}
