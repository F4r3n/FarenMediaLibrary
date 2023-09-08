#version 450

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform  SceneData{
    vec4 fogColor; // w is for exponent
	vec4 fogDistances; //x for min, y for max, zw unused.
	vec4 ambientColor;
	vec4 sunlightDirection; //w for sun power
	vec4 sunlightColor;
} sceneData;

void main() {
    vec3 ambient = sceneData.ambientColor.xyz;
    outColor = vec4(fragColor + ambient.xyz, 1.0);
    //outColor.xyz = ambient.xyz;
}
