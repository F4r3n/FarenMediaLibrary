#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normals;

uniform mat4 FM_M;
uniform mat4 FM_PVM;

layout(std140) uniform shader_data
{
	mat4 FM_V;
	mat4 FM_P;
	mat4 FM_PV;
	int render_mode;
};

out vec3 ourPosition;
out vec3 ourNormals;
out vec2 ourUVs;

void main()
{
	vec4 screenPos = vec4(position, 1.0f);
	gl_Position = FM_PVM * screenPos;
	ourPosition = (FM_M*screenPos).xyz;
	ourNormals = mat3(transpose(inverse(FM_M))) * normals;
	ourUVs = texCoords;
}