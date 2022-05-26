#version 460 core

out vec4 gl_FragColor;
in vec2 texCoords;
in flat int f_postProcessType;

uniform sampler2D screenTexture;
uniform sampler2D silhoutteTexture;


const float offset_x = 1.0f / 1380.0f;
const float offset_y = 1.0f / 780.0f;

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);


float kernelBlur[9] = float[](
    1.0 / 10, 2.0 / 10, 1.0 / 10,
    2.0 / 10, 4.0 / 10, 2.0 / 10,
    1.0 / 10, 2.0 / 10, 1.0 / 10  
);

float kernel[9] = float[](
    1, 1, 1,
    1, -8, 1,
    1, 1, 1 
);

void main()
{

  if(f_postProcessType == 0){

    vec4 color = vec4(0.0f);
    vec3 tmpColor = vec3(0.0f, 0.0f, 0.0f);
    tmpColor = texture(silhoutteTexture, texCoords.st).xyz;

    if(tmpColor.x != 0.0f || tmpColor.y != 0.0f || tmpColor.z != 0.0f){
      
       int countPixels = 0;
       for(int i = 0; i < 9; i++){
          vec3 tmpColorNeighbor = vec3(0.0f);
          //Excluimos el centro ya que lo hemos descartado
          if(i != 4){
            tmpColorNeighbor = vec3(texture(silhoutteTexture, texCoords.st + offsets[i]));
            if(tmpColorNeighbor.x != tmpColor.x || tmpColorNeighbor.y != tmpColor.y || tmpColorNeighbor.z != tmpColor.z){
              countPixels++;
            }
          }
          if(countPixels != 8 && countPixels > 0){
            color = texture(silhoutteTexture, texCoords.st);
          }
       }
       gl_FragColor = texture(screenTexture, texCoords) + color;
    }else{
      gl_FragColor = texture(screenTexture, texCoords);
    }

  }else if(f_postProcessType == 1){
	  //vec3 color = vec3(0.0f);
    //for(int i = 0; i < 9; i++)
        //color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];
    gl_FragColor = texture(silhoutteTexture, texCoords.st);
  }else if(f_postProcessType == 2){
	  vec3 color = vec3(0.0f);
    for(int i = 0; i < 9; i++)
        color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernelBlur[i];
    gl_FragColor = vec4(color, 1.0f);
  }
	
}
