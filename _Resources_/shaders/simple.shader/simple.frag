#version 450

out vec4 FragColor;
in vec2 TexCoords;

layout(binding=0) uniform sampler2D screenTexture;
layout(binding=1) uniform sampler2D bloomBlur;

uniform vec2 screenSize;
uniform vec2 viewPos;

void main()
 {
	vec4 hdrColor = texture(screenTexture, TexCoords.xy);
	FragColor = vec4(hdrColor.rgb, 1);
}