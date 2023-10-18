#pragma once
#include "MaterialValue.h"
#include "Core/FilePath.h"
#include "ShaderKind.hpp"
#include "Rendering/GraphicsAPI.h"
namespace fm
{
	class SubShader
	{
	public:

		struct Uniform
		{
			std::string name;
			int binding = -1;
			int set = -1;
			fm::ValuesType type = fm::ValuesType::VALUE_NONE;
		};

		struct Reflection
		{
			std::map<std::string, Uniform> uniforms;
		};
		using Reflections = std::array< fm::SubShader::Reflection, GRAPHIC_API::LAST>;

		SubShader() = delete;
		SubShader(const fm::FilePath& inPath, ShaderID inID);
		void Save(nlohmann::json& outJSON) const;
		void Load(const nlohmann::json& inJSON);
		void SetReflection(const Reflections& inReflection);
		const fm::FilePath& GetPath() const { return _path; }
		ShaderID	GetID() const { return _ID; }
		fm::SubShader::Reflection GetReflection(GRAPHIC_API inAPI) const { return _reflections[(size_t)inAPI]; }
	private:
		Reflections _reflections;
		fm::FilePath _path;
		ShaderID	 _ID = 0;
	};
}