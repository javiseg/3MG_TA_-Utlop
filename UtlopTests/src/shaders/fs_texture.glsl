#version 460 core
in vec2 text_coords;
in vec3 frag_position;
in vec3 frag_normal;
in vec3 FragPos;
in  flat int lightType;
in vec4 fragPosLight;

flat in int doesHasNormalMap;
in vec3 camPosition;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
uniform sampler2D normal0;

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

in PointLightStr pointLightSTR;
in DirectionalLightStr directionalLightSTR;

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
	
	vec3 lightDirection = normalize(directionalLightSTR.direction);
		
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

	float shadow = 0.0f;
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f){
		lightCoords = (lightCoords + 1.0f) / 2.0f;

		float closestDepth = texture(shadowMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;


		float bias = 0.005f;
		if(currentDepth > closestDepth + bias)
			shadow = 1.0f;
	}

	return (texture(diffuse0, text_coords) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, text_coords).r * specular * (1.0f - shadow)) * vec4(directionalLightSTR.color,1.0f);
}

vec4 SpotLight()
{
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	float ambient = 0.20f;

	vec3 normal = normalize(frag_normal);
	vec3 lightDirection = normalize(pointLightSTR.dirLightPos - frag_position);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	
  float specular = 0.0f;
	
  if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPosition - frag_position);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);


	float shadow = 0.0f;
	
  vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	
  if(lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		
    //float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.000005f);
    float bias = 0.005f;
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		
    for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		
    shadow /= pow((sampleRadius * 2 + 1), 2);

	}

	return (texture(diffuse0, text_coords) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, text_coords).r * specular * (1.0f - shadow) * inten) * vec4(pointLightSTR.color,1.0f);
}

void main()
{
  vec4 outputColor = vec4(0.0f);
	if(pointLightSTR.intensity == 0){
		vec4 outputColor = texture(diffuse0, text_coords);
		gl_FragColor = outputColor;
	}else{
		if(lightType == 0){
			outputColor += PointLight();
		}
    
    if(lightType == 1){
			outputColor += DirectionalLight();
		}
    if(lightType == 2){
      outputColor += SpotLight();
    }
	}
	gl_FragColor = outputColor;
}
