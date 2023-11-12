#pragma once
#include "MaterialValue.h"
#include "Core/FilePath.h"
#include "ShaderKind.hpp"
#include "Rendering/GraphicsAPI.h"
#include <array>
#include <vector>

namespace fm
{
	class SubShader
	{
	public:
		enum STAGE
		{
			NONE = 0,
			VERTEX = 1,
			FRAGMENT = 2,
			ALL = VERTEX | FRAGMENT
		};

		struct Variable
		{
			std::string name;
			std::string typeName;
			fm::ValuesType type = fm::ValuesType::VALUE_NONE;
			uint32_t size = 0;
			uint32_t offset = 0;
			bool isBlock = false;
		};

		struct Block
		{
			std::string name;
			uint32_t size = 0;
			std::vector<Variable> variables;
		};

		struct Uniform
		{
			std::string name;
			int binding = -1;
			int set = -1;
			int stages = (int)STAGE::ALL;
			std::vector<Variable> variables; //order is needed
			bool IsTexture() const { return (variables.size() == 1 && variables.front().type == fm::ValuesType::VALUE_TEXTURE); }
		};

		struct Reflection
		{
			std::map<std::string, Uniform> uniforms;
			std::map<std::string, Block> blocks;
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