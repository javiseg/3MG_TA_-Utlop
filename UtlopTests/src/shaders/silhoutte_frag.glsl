#version 460 core


out vec4 gl_FragColor;
in vec3 f_color;
uniform sampler2D silhoutteTexture;
in vec2 texCoords;


const float offset_x = 1.0f / 1380.0f;
const float offset_y = 1.0f / 780.0f;

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

float kernel[9] = float[](
    1, 1, 1,
    1, 1, 1,
    1, 1, 1 
);


void main()
{            
    vec3 color = f_color;
    vec3 tmpColor = vec3(0.0f, 0.0f, 0.0f);
    vec3 resultColor = vec3(0.0f, 0.0f, 0.0f);
    tmpColor = texture(silhoutteTexture, texCoords.st).xyz;

    if(tmpColor.x != 0.0f || tmpColor.y != 0.0f || tmpColor.z != 0.0f){
      
       int countPixels = 0;
       for(int i = 0; i < 9; i++){
          vec3 tmpColorNeighbor = vec3(0.0f);
          //Excluimos el centro ya que lo hemos descartado
          if(i != 4){
            tmpColorNeighbor = vec3(texture(silhoutteTexture, texCoords.st + offsets[i]));
            if(tmpColorNeighbor.x != tmpColor.x || tmpColorNeighbor.y != tmpColor.y || tmpColorNeighbor.z != tmpColor.z){
              resultColor = tmpColorNeighbor;
              countPixels++;
            }
          }
          if(countPixels != 8){
            color = resultColor;
            gl_FragColor = vec4(color, 1.0f);
          }
       }
    }
    //gl_FragColor = vec4(color, 1.0f);
   
    //for(int i = 0; i < 9; i++){
    //  tmpColor = vec3(texture(silhoutteTexture, texCoords.st + offsets[i]));
    //  if(tmpColor.b >= 0.5f)
    //    gl_FragColor = vec4(tmpColor, 1.0f);
    
    //}
      
}