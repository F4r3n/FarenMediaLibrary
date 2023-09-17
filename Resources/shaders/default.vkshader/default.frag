#version 450

layout(location = 0) in vec3 fragColor;
layout (location = 1) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 1) uniform  SceneData{
    vec4 fogColor; // w is for exponent
	vec4 fogDistances; //x for min, y for max, zw unused.
	vec4 ambientColor;
	vec4 sunlightDirection; //w for sun power
	vec4 sunlightColor;
} sceneData;

layout(set = 2, binding = 0) uniform sampler2D tex1;

void main() {
    vec3 ambient = sceneData.ambientColor.xyz;
    vec3 color = texture(tex1,texCoord).xyz;
    outColor = vec4(fragColor*color, 1.0);

    //vec3 color = texture(tex1, texCoord).xyz;
	//outColor = vec4(color,1.0f);
    //outColor.xyz = ambient.xyz;
}
