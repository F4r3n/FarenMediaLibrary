#version 450
#extension GL_GOOGLE_include_directive : require
#include "uniforms.glsl"

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;
layout(location = 0) out vec4 outColor;

UNIFORM(0, 1) SceneData{
    vec4 fogColor; // w is for exponent
	vec4 fogDistances; //x for min, y for max, zw unused.
	vec4 ambientColor;
	vec4 sunlightDirection; //w for sun power
	vec4 sunlightColor;
} sceneData;

UNIFORM_SAMPLER(2, 0) tex1;

void main() {
    vec3 ambient = sceneData.ambientColor.xyz;
    vec3 color = texture(tex1,texCoord).xyz;
    outColor = vec4(fragColor*color, 1.0);
}
