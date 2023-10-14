#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;

uniform mat4 FM_M;

layout(set = 0, binding = 0) uniform  CameraBuffer{
	mat4 view;
	mat4 proj;
	mat4 viewproj;
} cameraData;

out vec3 ourPosition;
out vec3 ourNormals;
out vec2 ourUVs;

void main() 
{
	mat4 transformMatrix = (cameraData.viewproj * FM_M);
	gl_Position = transformMatrix * vec4(vPosition, 1.0f);
	
	vec4 screenPos = vec4(vPosition, 1.0f);
	ourPosition = (FM_M*screenPos).xyz;
	ourNormals = mat3(transpose(inverse(FM_M))) * vNormal;
	ourUVs = vTexCoord;
}