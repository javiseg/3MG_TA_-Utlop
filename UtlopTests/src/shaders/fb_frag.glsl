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
    gl_FragColor = texture(screenTexture, texCoords);
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
