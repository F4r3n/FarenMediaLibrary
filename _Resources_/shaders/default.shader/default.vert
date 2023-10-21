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
layout (location = 3) out vec3 outColors;

UNIFORM(0, 0) _CameraBuffer{
	mat4 view;
	mat4 proj;
	mat4 viewproj;
} cameraData;

struct ObjectData{
	mat4 model;
};

READ_ONLY_BUFFER(1, 2) _ObjectBuffer{
	ObjectData objects[];
} objectBuffer;

struct UserData
{
	vec4 info1;
};

UNIFORM(0, 3) MaterialBuffer {
	UserData data;
} materialBuffer;


void main()
{
	mat4 modelMatrix = objectBuffer.objects[INSTANCE_INDEX].model;
	mat4 transformMatrix = (cameraData.viewproj * modelMatrix);
	gl_Position = transformMatrix * vec4(vPosition, 1.0f);

	outNormals = mat3(transpose(inverse(modelMatrix))) * vNormal;
	outUVs = vTexCoord;
	outColors = vColor;
	outPosition = gl_Position.xyz;
}