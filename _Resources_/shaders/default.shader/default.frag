#version 460
#extension GL_GOOGLE_include_directive : require
#include "uniforms.glsl"


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormals;
layout (location = 2) in vec2 inUVs;
layout (location = 3) in vec3 inColors;


layout(location = 0) out vec4 outColor;

UNIFORM(0, 1) _SceneData{
    vec4 fogColor; // w is for exponent
	vec4 fogDistances; //x for min, y for max, zw unused.
	vec4 ambientColor;
	vec4 sunlightDirection; //w for sun power
	vec4 sunlightColor;
} sceneData;

#ifdef _TEXTURE_
UNIFORM_SAMPLER(2, 0) TEXTURE_tex1;
#endif

void main() {
#ifdef _TEXTURE_
    vec4 color = vec4(inColors*texture(TEXTURE_tex1,inUVs).xyz, 1.0);
#else
	vec4 color = vec4(inColors, 1.0);
#endif
    outColor = color;
}
