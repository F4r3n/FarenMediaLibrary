#pragma once
#include <memory>
#include <unordered_map>
#include "Rendering/ShaderKind.hpp"
namespace fm
{
	class SubShader;
	class OGLShader;
	struct OGLShaderCache
	{
		std::unordered_map<fm::ShaderID, std::shared_ptr<fm::OGLShader>> _shaders;
		std::shared_ptr<fm::OGLShader> FindOrCreate(const fm::SubShader& inShader);
		void Release(uint32_t inID);
		~OGLShaderCache();
	};
}
