#version 420 core
//#include "../define.shader"
//#define UNIFORM0 layout(set=2, binding=0) uniform

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormals;
layout (location = 2) out vec2 outUVs;

uniform mat4 FM_M;


layout(set = 0, binding = 0) uniform  CameraBuffer{
	mat4 view;
	mat4 proj;
	mat4 viewproj;
} cameraData;


void main()
{
	mat4 transformMatrix = (cameraData.viewproj * FM_M);
	gl_Position = transformMatrix * vec4(vPosition, 1.0f);
	
	vec4 screenPos = vec4(vPosition, 1.0f);
	//gl_Position = FM_PVM * screenPos;
	outPosition = (FM_M*screenPos).xyz;
	outNormals = mat3(transpose(inverse(FM_M))) * vNormal;
	outUVs = vTexCoord;
}