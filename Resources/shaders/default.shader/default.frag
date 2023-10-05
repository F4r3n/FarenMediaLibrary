#version 420 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;
layout(location = 2) out vec4 posTexture;

layout (location = 0) in vec3 ourPosition;
layout (location = 1) in vec3 ourNormals;
layout (location = 2) in vec2 ourUVs;

uniform vec4 mainColor;
uniform int BloomEffect;
uniform sampler2D texture0;

void main() 
{
	vec4 color = mainColor;
	posTexture = vec4(ourPosition, 1);
	FragColor = color * texture(texture0, ourUVs);
	FragColor = color;
}