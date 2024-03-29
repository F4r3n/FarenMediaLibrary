#version 460

layout(location = 0) in vec4 vPosition;

out vec2 TexCoords;
uniform mat4 PM;

layout(std140) uniform shader_data
{
	mat4 FM_V;
	mat4 FM_P;
	mat4 FM_VP;
};

void main() 
{
	gl_Position = PM*vec4(vPosition.xy, 0.0, 1.0);
	TexCoords = vPosition.zw;
}