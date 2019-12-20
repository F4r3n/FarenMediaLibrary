#include "Rendering/Shader.h"
#include "Rendering/ShaderLibrary.h"
#include "Core/Config.h"
#include "Rendering/ShaderParser.h"
#include "Resource/ResourcesManager.h"
#if OPENGL_CORE == 1
#define SHADER_VERSION "#version 420 core\n"
#else
#define SHADER_VERSION "#version 310 es\n"
#endif


#define C(v, s) v s 
#define S(...) C(SHADER_VERSION, #__VA_ARGS__)
#define STRING(...) S(__VA_ARGS__)
ShaderLibrary::ShaderLibrary() {

}

ShaderLibrary::~ShaderLibrary() 
{
}

void ShaderLibrary::LoadShaders() 
{
	std::string text_vertex = STRING(
		layout(location = 0) in vec2 position;
	layout(location = 1) in vec2 texCoords;
	out vec2 TexCoords;
	uniform mat4 projection;
	layout(std140) uniform shader_data
	{
		mat4 FM_V;
		mat4 FM_P;
		mat4 FM_VP;
	};
	void main() {
		gl_Position = projection * vec4(position, 1.0, 1.0);
		TexCoords = texCoords;
	});

	std::string text_fragment = STRING(

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

	void main() {
		vec3 offset = vec3(0.0, 1.3846153846, 3.2307692308);
		vec3 weight = vec3(0.2270270270, 0.3162162162, 0.0702702703);
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
		vec4 color = textColor * sampled;

		vec2 tex_offset = 1.0f / vec2(textureSize(text, 0));
		float result = texture(text, TexCoords).r * weight[0];
		if (soft_edges || outline) {
			for (int i = 1; i < 3; ++i)
			{
				result += texture(text, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];
				result += texture(text, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).r * weight[i];

				result += texture(text, TexCoords + vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
				result += texture(text, TexCoords - vec2(0.0, tex_offset.y * offset[i])).r * weight[i];
			}
		}
		float oFactor = 1.0f;

		if (outline && result >= outline_min.x && result <= outline_max.y) {
			if (result <= outline_min.y) {
				oFactor = smoothstep(outline_min.x, outline_min.y, result);
			}
			else {
				oFactor = smoothstep(outline_max.x, outline_max.y, result);
			}
			color = mix(color, vec4(outline_color, 1), oFactor);
		}
		if (soft_edges) {
			color.a += smoothstep(soft_edge_values.x, soft_edge_values.y, result);
		}

		FragColor = color;
	});

	std::string instancing_vertex = STRING(
		layout(location = 0) in vec2 position;
	layout(location = 1) in vec3 color;
	layout(location = 3) in mat4 instanceMatrix;
	out vec2 TexCoords;
	out vec3 Color;
	uniform mat4 view;
	uniform mat4 projection;

	layout(std140) uniform shader_data
	{
		mat4 FM_V;
		mat4 FM_P;
		mat4 FM_VP;
		int render_mode;
	};
	void main() {
		vec2 colorTest = vec2(1);
		gl_Position = projection * view*instanceMatrix*vec4(position, 0.0f, 1.0f);
		Color = color;
		TexCoords = colorTest;
	});

	std::string instancing_fragment = STRING(

		in vec2 TexCoords;
	in vec3 Color;
	out vec4 color;
	void main() {
		color = vec4(1);
	});

	std::string blur_fragment = STRING(

		out vec4 FragColor;
	in  vec2 TexCoords;

	uniform sampler2D image;
	uniform int horizontal;

	void main()
	{
		vec3 offset = vec3(0.0, 1.3846153846, 3.2307692308);
		vec3 weight = vec3(0.2270270270, 0.3162162162, 0.0702702703);
		vec2 tex_offset = 1.0f / vec2(textureSize(image, 0));
		vec3 result = texture(image, TexCoords).rgb * weight[0];
		if (horizontal == 1)
		{
			for (int i = 1; i < 3; ++i)
			{
				result += texture(image, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
				result += texture(image, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
			}
		}
		else
		{
			for (int i = 1; i < 3; ++i)
			{
				result += texture(image, TexCoords + vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
				result += texture(image, TexCoords - vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
			}
		}
		FragColor = vec4(result, 1.0);
	});


	std::string light_fragment_deferred = STRING(

	layout(location = 0) out vec4 FragColor;
	layout(location = 1) out vec4 BrightColor;

	in vec2 TexCoords;
	layout(binding=0) uniform sampler2D screenTexture;
	layout(binding=1) uniform sampler2D posTexture;

	uniform vec2 screenSize;
	uniform vec2 viewPos;

	struct PointLight {
		vec3 position;
		vec4 color;
		int ready;
		float radius;
	};

	struct DirectionalLight {
		vec4 color;
	};

	const int MAX_LIGHTS = 32;

	uniform PointLight light[MAX_LIGHTS];
	uniform DirectionalLight dlight;
	void main() {

		vec4 hdrColor = texture(screenTexture, TexCoords);
		vec4 pos = texture(posTexture, TexCoords);
		vec3 result = dlight.color.rgb*hdrColor.rgb;

		for (int i = 0; i < MAX_LIGHTS; i++) {
			vec3 dir = vec3(1);
			if (light[i].ready == 1) {
				dir = normalize(light[i].position - pos.rgb);
			}
			float ambientStrength = 0.2f;
			vec3 ambient = ambientStrength * light[i].color.rgb;

			vec3 norm = normalize(vec3(0, 0, 1));

			float diff = max(dot(norm, dir), 0.0);
			vec3 diffuse = diff * light[i].color.rgb;
			float distance = length(light[i].position - pos.rgb);
			float attenuation = 1.0f / (0.9f * distance + 0.0032f * (distance * distance));
			result += (ambient*attenuation*255.0f + diffuse * attenuation*255.0f)*hdrColor.rgb;
		}
		BrightColor = vec4(0, 0, 0, 1);
		FragColor = vec4(result, 1);
		//FragColor = vec4(1);

		float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
		if (brightness >= 0.5)
			BrightColor = vec4(FragColor.rgb, 1.0);
	});

	std::string light_fragment_forward = STRING(

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

	void main() {
		vec4 color = mainColor;
		posTexture = vec4(ourPosition, 1);
		vec3 result = vec3(0);
		//FragColor = color * texture(texture0, ourUVs);
		for (int i = 0; i < lightNumber; i++) {
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

	
	);

	std::string light_fragment_no_light = STRING(

		layout(location = 0) out vec4 FragColor;
	layout(location = 1) out vec4 BrightColor;

	in vec2 TexCoords;
	layout(binding=0) uniform sampler2D screenTexture;
	layout(binding=1) uniform sampler2D posTexture;

	uniform vec2 screenSize;
	uniform vec2 viewPos;


	void main() {

		vec4 hdrColor = texture(screenTexture, TexCoords);
		vec4 pos = texture(posTexture, TexCoords);

		FragColor = hdrColor;

	});

	std::string simple_vertex = STRING(
		layout(location = 0) in vec3 position;
	layout(location = 1) in vec2 texCoords;
	out vec2 TexCoords;
	layout(std140) uniform shader_data
	{
		mat4 FM_V;
		mat4 FM_P;
		mat4 FM_VP;
		int render_mode;
	};
	uniform int  reverse;
	void main()
	{
		gl_Position = vec4(position, 1.0f);
		if (reverse == 0)
		{
			TexCoords = texCoords;
		}
		else
		{
			TexCoords.x = texCoords.x;
			TexCoords.y = 1 - texCoords.y;

		}

	});

	std::string simple_fragment = STRING(

		out vec4 FragColor;
	in vec2 TexCoords;
	layout(binding=0) uniform sampler2D screenTexture;
	layout(binding=1) uniform sampler2D bloomBlur;

	uniform vec2 screenSize;
	uniform vec2 viewPos;

	void main() {

		vec4 hdrColor = texture2D(screenTexture, vec2(TexCoords.x, TexCoords.y));
		FragColor = vec4(hdrColor.rgb, 1);
		//FragColor = vec4(1);

	});

	std::string default_vertex = STRING(
		layout(location = 0) in vec3 position;
	layout(location = 1) in vec2 texCoords;
	layout(location = 2) in vec3 normals;

	uniform mat4 FM_M;
	uniform mat4 FM_PVM;

	layout(std140) uniform shader_data
	{
		mat4 FM_V;
		mat4 FM_P;
		mat4 FM_PV;
		int render_mode;

	};

	out vec3 ourPosition;
	out vec3 ourNormals;
	out vec2 ourUVs;
	void main() {

		vec4 screenPos = vec4(position, 1.0f);
		gl_Position = FM_PVM * screenPos;
		ourPosition = (FM_M*screenPos).xyz;
		ourNormals = mat3(transpose(inverse(FM_M))) * normals;
		ourUVs = texCoords;
	});

	std::string default_fragment = STRING(

	layout(location = 0) out vec4 FragColor;
	layout(location = 1) out vec4 BrightColor;
	layout(location = 2) out vec4 posTexture;

	uniform vec4 mainColor;
	uniform int BloomEffect;
	in vec3 ourPosition;
	in vec2 ourUVs;
	uniform sampler2D texture0;

	void main() {
		vec4 color = mainColor;
		posTexture = vec4(ourPosition, 1);
		FragColor = color * texture(texture0, ourUVs);
		FragColor = color;

	});



	std::string default_vertex_sprite = STRING(
		layout(location = 0) in vec2 position;
	layout(location = 1) in vec2 texCoords;


	uniform mat4 model;


	uniform mat4 FM_PV;

	out vec4 ourColor;
	out vec2 ourTexCoord;
	out vec3 ourPosition;
	layout(std140) uniform shader_data
	{
		mat4 FM_V;
		mat4 FM_P;
		mat4 FM_VP;
	};
	void main() {
		vec4 worldPos = model * vec4(position, 1.0f, 1.0f);
		gl_Position = FM_PV * worldPos;
		ourPosition = worldPos.xyz;
		ourTexCoord = texCoords;
	});

	std::string default_fragment_sprite = STRING(

	layout(location = 0) out vec4 FragColor;
	layout(location = 1) out vec4 BrightColor;
	layout(location = 2) out vec4 posTexture;

	uniform vec4 mainColor;
	in vec2 ourTexCoord;
	in vec3 ourPosition;
	uniform int BloomEffect;
	uniform sampler2D texture2d;
	out vec4 Color;

	void main() {
		vec4 color = texture(texture2d, ourTexCoord)*mainColor;
		BrightColor = vec4(0, 0, 0, 1);
		posTexture = vec4(ourPosition, 0);

		if (color.w == 0.0f) discard;



		BrightColor = vec4(0, 0, 0, 1); FragColor = vec4(0);
		if (BloomEffect <= 0) {
			FragColor = color;
		}
		else {
			BrightColor = color;
		}
	});

	std::string default_vertex_particle = STRING(

	layout(location = 0) in vec4 vertex;
	uniform mat4 view;
	uniform mat4 projection;
	uniform float scale;
	uniform vec2 offset;
	uniform vec4 particleColor;

	out vec4 ourColor;
	out vec2 ourTexCoord;
	layout(std140) uniform shader_data
	{
		mat4 FM_V;
		mat4 FM_P;
		mat4 FM_VP;
	};
	void main() {
		gl_Position = projection * view*vec4((vertex.xy*scale) + offset, 0.0f, 1.0f);
		ourTexCoord = vertex.zw;
		ourColor = particleColor;
	});

	std::string default_fragment_particle = STRING(

	in vec4 ourColor;
	in vec2 ourTexCoord;
	uniform sampler2D texture2d;
	layout(location = 0) out vec4 FragColor;
	layout(location = 1) out vec4 BrightColor;
	void main() {
		FragColor = texture(texture2d, ourTexCoord)*ourColor;
	});


	fm::ResourcesManager::get().load<fm::Shader>("light_deferred", new fm::Shader(simple_vertex, light_fragment_deferred, "light_deferred"));
	fm::ResourcesManager::get().load<fm::Shader>("no_light", new fm::Shader(simple_vertex, light_fragment_no_light, "no_light"));
	fm::ResourcesManager::get().load<fm::Shader>("blur", new fm::Shader(simple_vertex, blur_fragment, "blur"));
	fm::ResourcesManager::get().load<fm::Shader>("text", new fm::Shader(text_vertex, text_fragment, "text"));
	fm::ResourcesManager::get().load<fm::Shader>("instancing", new fm::Shader(instancing_vertex, instancing_fragment, "instancing"));
	fm::ResourcesManager::get().load<fm::Shader>("default", new fm::Shader(default_vertex, default_fragment, "default"));
	fm::ResourcesManager::get().load<fm::Shader>("light_forward", new fm::Shader(default_vertex, light_fragment_forward, "light_forward"));
	fm::ResourcesManager::get().load<fm::Shader>("simple", new fm::Shader(simple_vertex, simple_fragment, "simple"));

	for (auto shader : fm::ResourcesManager::get().getAll<fm::Shader>()) 
	{
		fm::Shader *s = static_cast<fm::Shader*>(shader.second);
		if (!s->IsReady()) 
		{
			s->compile();
		}
	}
}

void ShaderLibrary::LoadShader(const std::string& name, const std::string &path) 
{
	fm::ShaderParser parser;

	std::tuple<std::string, std::string> partShader = parser.parse(path);
	fm::ResourcesManager::get().load<fm::Shader>(name, new fm::Shader(std::get<0>(partShader), std::get<1>(partShader), name));
}
