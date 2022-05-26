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
    
    gl_FragColor = vec4(f_color, 1.0f);
   
    //for(int i = 0; i < 9; i++){
    //  tmpColor = vec3(texture(silhoutteTexture, texCoords.st + offsets[i]));
    //  if(tmpColor.b >= 0.5f)
    //    gl_FragColor = vec4(tmpColor, 1.0f);
    
    //}
      
}