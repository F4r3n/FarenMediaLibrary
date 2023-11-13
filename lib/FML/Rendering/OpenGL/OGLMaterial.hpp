#pragma once
#include <string>
#include <memory>
#include "Rendering/OpenGL/OGLUniformbuffer.hpp"
#include "Rendering/material.hpp"
namespace fm
{
	struct TextureCache;
}

namespace fm
{
	class OGLShader;
	class OGLTexture;
	struct OGLMaterialCreateInfo {
		std::shared_ptr<fm::Material> material;
		std::shared_ptr<fm::OGLShader> shader;
	};

	class OGLMaterial
	{
	public:
		OGLMaterial(const OGLMaterialCreateInfo& inInfo, fm::TextureCache& inTextures);
		void Bind(const fm::MaterialValues& inMaterialProperties, fm::TextureCache& ioTextures);
		uint32_t GetID() const;

		void Destroy();
	private:
		std::shared_ptr<Material>							_material = nullptr;
		std::shared_ptr<OGLShader>							_shader = nullptr;
		fm::OGLUniformbuffer								_materialBuffer;
		std::vector<Material::MaterialBufferInfo_Texture>	_texturesInfo;
		size_t _materialStamp = 0;
	};
}