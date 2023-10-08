#version 420 core

out vec4 FragColor;
in vec2 TexCoords;

layout(binding=0) uniform sampler2D screenTexture;
layout(binding=1) uniform sampler2D bloomBlur;

uniform vec2 screenSize;
uniform vec2 viewPos;

void main()
 {
	vec4 hdrColor = texture2D(screenTexture, vec2(TexCoords.x, TexCoords.y));
	FragColor = vec4(hdrColor.rgb, 1);
}