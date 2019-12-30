#version 420 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;
uniform mat4 projection;

layout(std140) uniform shader_data
{
	mat4 FM_V;
	mat4 FM_P;
	mat4 FM_VP;
};

void main() 
{
	gl_Position = projection * vec4(position, 1.0, 1.0);
	TexCoords = texCoords;
}