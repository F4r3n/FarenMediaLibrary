#include "OGLMaterialCache.hpp"
#include "OGLMaterial.hpp"
#include "OGLShaderCache.hpp"
#include "Rendering/material.hpp"
#include <cassert>
using namespace fm;

std::shared_ptr<fm::OGLMaterial> OGLMaterialCache::FindOrCreate(std::shared_ptr<fm::Material> inMaterial,
	OGLShaderCache& ioShaderCache, OGLTextureCache& ioTextureCache)
{
	if (auto it = _materials.find(inMaterial->GetObjectID()); it != _materials.end())
	{
		return it->second;
	}

	std::optional<fm::SubShader> sub = inMaterial->GetSubShader();
	if (!sub.has_value())
	{
		assert(false);
	}

	std::shared_ptr<fm::OGLShader> shader = ioShaderCache.FindOrCreate(sub.value());
	if (shader == nullptr)
		return nullptr;

	fm::OGLMaterialCreateInfo info;
	info.material = inMaterial;
	info.shader = shader;
	std::unique_ptr<fm::OGLMaterial> mat = std::make_unique< fm::OGLMaterial>(info, ioTextureCache);
	std::shared_ptr<fm::OGLMaterial> material = _materials.emplace(inMaterial->GetObjectID(), std::move(mat)).first->second;

	inMaterial->SetDestroyCallback(std::bind(&OGLMaterialCache::Release, this, std::placeholders::_1));

	return material;
}

OGLMaterialCache::~OGLMaterialCache()
{
	for (auto& mat : _materials)
	{
		mat.second->Destroy();
	}
}


void OGLMaterialCache::Release(uint32_t inID)
{
	if (auto it = _materials.find(inID); it != _materials.end())
	{
		_materials.erase(it);
	}
}

