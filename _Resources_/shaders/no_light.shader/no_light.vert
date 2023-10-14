#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;



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
	gl_Position = vec4(vPosition, 1.0f);
	if (reverse == 0)
	{
		TexCoords = vTexCoord;
	}
	else
	{
		TexCoords.x = vTexCoord.x;
		TexCoords.y = 1 - vTexCoord.y;
	}
}