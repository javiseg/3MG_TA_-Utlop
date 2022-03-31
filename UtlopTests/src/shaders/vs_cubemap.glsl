#version 460 core
layout (location = 0) in vec3 aPos;

layout(std140, binding = 0) uniform PerFrameData {
  uniform mat4 model;
  uniform mat4 MVP;
  uniform vec4 cameraPos;
};
struct PerVertex {
 vec2 uv;
 vec3 normal;
 vec3 worldPos;
};

layout (location=0) out PerVertex vtx;

void main() {
  vec3 pos = getPosition(gl_VertexID);
  gl_Position = MVP * vec4(pos, 1.0);
  mat3 normalMatrix = mat3(transpose(inverse(model)));
  vtx.uv = getTexCoord(gl_VertexID);
  vtx.normal = getNormal(gl_VertexID) * normalMatrix;
  vtx.worldPos = (model * vec4(pos, 1.0)).xyz;
}
