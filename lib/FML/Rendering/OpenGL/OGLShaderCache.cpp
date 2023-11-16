#include "OGLShaderCache.hpp"
#include "OGLShader.hpp"
#include "Rendering/SubShader.hpp"
#include <cassert>
using namespace fm;

std::shared_ptr<fm::OGLShader> OGLShaderCache::FindOrCreate(const fm::SubShader& inShader)
{
	fm::ShaderID ID = inShader.GetID();
	auto it = _shaders.find(ID);
	if (it != _shaders.end())
		return it->second;

	std::shared_ptr<fm::OGLShader> shader = std::make_shared<fm::OGLShader>(inShader);
	_shaders.emplace(ID, shader);
	shader->compile();
	return shader;
}

OGLShaderCache::~OGLShaderCache()
{
	_shaders.clear();
}


void OGLShaderCache::Release(uint32_t inID)
{
	_shaders.erase(inID);
}

