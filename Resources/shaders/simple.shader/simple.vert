#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
out vec2 TexCoords;

layout(std140) uniform shader_data
{
	mat4 FM_V;
	mat4 FM_P;
	mat4 FM_VP;
	int render_mode;
};

uniform int reverse;

void main()
{
	gl_Position = vec4(position, 1.0f);
	if (reverse == 0)
	{
		TexCoords = texCoords;
	}
	else
	{
		TexCoords.x = texCoords.x;
		TexCoords.y = 1 - texCoords.y;
	}
}