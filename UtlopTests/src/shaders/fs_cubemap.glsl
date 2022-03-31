layout (binding = 0) uniform sampler2D texture0;
layout (binding = 1) uniform samplerCube texture1;
void main() {
  vec3 n = normalize(vtx.normal);
  vec3 v = normalize(cameraPos.xyz - vtx.worldPos);
  vec3 reflection = -normalize(reflect(v, n));
  float eta = 1.00 / 1.31;
  vec3 refraction = -normalize(refract(v, n, eta));
  const float R0 = ((1.0-eta) * (1.0-eta)) /
                   ((1.0+eta) * (1.0+eta));
  const float Rtheta = R0 + (1.0 – R0) * pow((1.0 – dot(-v, n)), 5.0);
  vec4 color = texture(texture0, vtx.uv);
  vec4 colorRefl = texture(texture1, reflection);
  vec4 colorRefr = texture(texture1, refraction);
  color = color * mix(colorRefl, colorRefr, Rtheta);
  out_FragColor = color;
};