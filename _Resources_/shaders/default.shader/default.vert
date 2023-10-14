#version 460
#extension GL_GOOGLE_include_directive : require
#include "uniforms.glsl"

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormals;
layout (location = 2) out vec2 outUVs;

uniform mat4 FM_M;


UNIFORM(0, 0) CameraBuffer{
	mat4 view;
	mat4 proj;
	mat4 viewproj;
} cameraData;

struct ObjectData{
	mat4 model;
};

READ_ONLY_BUFFER(1, 2) ObjectBuffer{
	ObjectData objects[];
} objectBuffer;


void main()
{
	mat4 modelMatrix = objectBuffer.objects[gl_BaseInstance + gl_InstanceID].model;
	mat4 transformMatrix = (cameraData.viewproj * modelMatrix);
	gl_Position = transformMatrix * vec4(vPosition, 1.0f);

	//mat4 transformMatrix = (cameraData.viewproj * FM_M);
	//gl_Position = transformMatrix * vec4(vPosition, 1.0f);
	
	vec4 screenPos = vec4(vPosition, 1.0f);
	//gl_Position = FM_PVM * screenPos;
	outPosition = (FM_M*screenPos).xyz;
	outNormals = mat3(transpose(inverse(FM_M))) * vNormal;
	outUVs = vTexCoord;
}