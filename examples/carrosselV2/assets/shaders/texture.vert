#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

uniform vec4 GroundColor;

uniform vec4 lightDirWorldSpace;

out vec3 fragV;
out vec3 fragL;
out vec3 fragN;
out vec2 fragTexCoord;
out vec3 fragPObj;
out vec3 fragNObj;

out vec4 fragColor;

void main() {
  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;
  vec3 L = -(viewMatrix * lightDirWorldSpace).xyz;

  fragL = L;
  fragV = -P;
  fragN = N;
  fragTexCoord = inTexCoord;
  fragPObj = inPosition;
  fragNObj = inNormal;

  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);
  float i = 1.0 - (-posEyeSpace.z / 15.0);
  fragColor = vec4(i, i, i, 1) * GroundColor;

  gl_Position = projMatrix * vec4(P, 1.0);
}
