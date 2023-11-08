#include "uniforms.glsl"

struct PLAIN {
	vec4 mainColor;
};

UNIFORM(2, 3) MaterialBuffer {
	PLAIN plain;
	uvec4 _properties;
} materialBuffer;

UNIFORM_SAMPLER(3, 4) TEXTURE_albedo;

bool has_properties() {return materialBuffer._properties.x != 0;}
bool has_texture_albedo() { return (materialBuffer._properties.x & uint(1U << 1)) != 0; }