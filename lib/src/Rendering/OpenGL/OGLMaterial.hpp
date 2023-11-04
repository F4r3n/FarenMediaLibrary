#pragma once
#include <string>
#include <memory>
#include "Rendering/OpenGL/OGLUniformbuffer.hpp"
namespace fm
{
	class Material;
	class OGLShader;
	class MaterialProperties;
	struct OGLMaterialCreateInfo {
		std::shared_ptr<fm::Material> material;
		std::shared_ptr<fm::OGLShader> shader;
	};

	class OGLMaterial
	{
	public:
		OGLMaterial(const OGLMaterialCreateInfo& inInfo);
		void Bind(const fm::MaterialProperties& inMaterialProperties);
		uint32_t GetID() const;

		void Destroy();
	private:
		std::shared_ptr<Material>	_material = nullptr;
		std::shared_ptr<OGLShader>	_shader = nullptr;
		fm::OGLUniformbuffer		_materialBuffer;

		size_t _materialStamp = 0;
	};
}