#include "uniforms.glsl"

struct PLAIN {
	vec4 mainColor;
};

UNIFORM(2, 3) MaterialBuffer {
	PLAIN plain;
	uvec4 _properties;
} materialBuffer;

bool has_texture_albedo() { return (materialBuffer._properties.x & uint(1U << 0)) == 0; }