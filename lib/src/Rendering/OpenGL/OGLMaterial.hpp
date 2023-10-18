#pragma once
#include <string>
#include <memory>

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
	private:
		std::shared_ptr<Material>	_material = nullptr;
		std::shared_ptr<OGLShader>	_shader = nullptr;
	};
}