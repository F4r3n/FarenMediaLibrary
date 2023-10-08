#version 420 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;
layout(location = 2) out vec4 posTexture;

uniform vec4 mainColor;
in vec3 ourPosition;
in vec3 ourNormals;
in vec2 ourUVs;
uniform vec3 viewPos;
uniform int lightNumber;

struct PointLight {
	vec4 position;
	vec4 color;
	vec4 custom;
};

struct DirectionalLight {
	vec4 color;
};

const int MAX_LIGHTS = 32;

layout(std140) uniform PointLights
{
	PointLight light[MAX_LIGHTS];
};
uniform DirectionalLight dlight;

void main()
{
	vec4 color = mainColor;
	posTexture = vec4(ourPosition, 1);
	vec3 result = vec3(0);
		//FragColor = color * texture(texture0, ourUVs);
	for (int i = 0; i < lightNumber; i++) 
	{
		PointLight pl = light[i];
		vec3 norm = normalize(ourNormals);
		vec3 lightDir = normalize(pl.position.xyz - ourPosition);

		float diff = max(dot(norm, lightDir), 0.0);

		vec3 viewDir = normalize(viewPos - ourPosition);
		//vec3 halfwayDir = normalize(lightDir + viewDir);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(norm, reflectDir), 0.0), 32);

		vec3 specular = 0.5*spec*pl.color.xyz;
		vec3 diffuse = diff * pl.color.xyz;

		result += diffuse + specular + vec3(0.1f);
		//result = abs(ourNormals);
	}

	FragColor = vec4(result*color.rgb , 1);

}