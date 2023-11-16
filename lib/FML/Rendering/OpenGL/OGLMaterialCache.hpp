#pragma once
#include <memory>
#include <unordered_map>
namespace fm
{
	class OGLMaterial;
	class Material;
	class OGLShader;
	struct OGLTextureCache;
	struct OGLShaderCache;
	struct OGLMaterialCache
	{
		std::unordered_map<uint32_t, std::shared_ptr<fm::OGLMaterial>> _materials;
		std::shared_ptr<fm::OGLMaterial> FindOrCreate(std::shared_ptr<fm::Material> inMaterial,
			OGLShaderCache& ioShaderCache, OGLTextureCache& ioTextureCache );
		void Release(uint32_t inID);
		~OGLMaterialCache();
	};
}
