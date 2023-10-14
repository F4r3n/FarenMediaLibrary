#version 450

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;
layout(location = 2) out vec4 posTexture;

uniform vec4 mainColor;
uniform int BloomEffect;

uniform vec4 colorID;


in vec3 ourPosition;
in vec2 ourUVs;

uniform sampler2D texture0;

void main() 
{
	vec4 color = mainColor;
	posTexture = vec4(ourPosition, 1);
	FragColor = colorID;
	//FragColor = vec4(1,0,0,1);
}