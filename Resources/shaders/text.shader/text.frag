#version 420 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;
layout(location = 2) out vec4 posTexture;

in vec2 TexCoords;
uniform sampler2D text;
uniform vec4 textColor;

uniform bool outline;
uniform vec2 outline_min;
uniform vec2 outline_max;
uniform vec3 outline_color;

uniform bool soft_edges;
uniform vec2 soft_edge_values;

void main() 
{
	vec3 offset = vec3(0.0, 1.3846153846, 3.2307692308);
	vec3 weight = vec3(0.2270270270, 0.3162162162, 0.0702702703);
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	vec4 color = textColor * sampled;

	vec2 tex_offset = 1.0f / vec2(textureSize(text, 0));
	float result = texture(text, TexCoords).r * weight[0];
	if (soft_edges || outline) 
	{
		for (int i = 1; i < 3; ++i)
		{
			result += texture(text, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];
			result += texture(text, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];

			result += texture(text, TexCoords + vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
			result += texture(text, TexCoords - vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
		}
	}

	float oFactor = 1.0f;

	if (outline && result >= outline_min.x && result <= outline_max.y) 
	{
		if (result <= outline_min.y) 
		{
			oFactor = smoothstep(outline_min.x, outline_min.y, result);
		}
		else 
		{
			oFactor = smoothstep(outline_max.x, outline_max.y, result);
		}

		color = mix(color, vec4(outline_color, 1), oFactor);
	}
		
	if (soft_edges) 
	{
		color.a += smoothstep(soft_edge_values.x, soft_edge_values.y, result);
	}

	FragColor = color;
}