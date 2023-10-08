#version 420 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in vec2 TexCoords;
layout(binding=0) uniform sampler2D screenTexture;
layout(binding=1) uniform sampler2D posTexture;

uniform vec2 screenSize;
uniform vec2 viewPos;


void main() 
{
	vec4 hdrColor = texture(screenTexture, TexCoords);
	vec4 pos = texture(posTexture, TexCoords);

	FragColor = hdrColor;
}