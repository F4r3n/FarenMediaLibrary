
#include "Rendering/Shader.h"
#include <iostream>
#include "Rendering/MaterialValue.h"
#include "Core/Color.h"
#include "Core/FilePath.h"
#include "SubShader.hpp"
#include <nlohmann/json.hpp>

using namespace fm;



Shader::Shader(const fm::FilePath& inFilePath) : Resource(inFilePath)
{
	_name = inFilePath.GetName(true);
	_ID++;
	_currentID = _ID;
}

void Shader::Save(nlohmann::json& outJSON) const
{
	fm::Resource::Save(outJSON);

	nlohmann::json& s = outJSON["subShaders"];

	for (const auto& [type, subshader] : _subShaders)
	{
		nlohmann::json subShaderJSON;
		subshader.Save(subShaderJSON);

		nlohmann::json j;
		j["kind"] = type;
		j["data"] = subShaderJSON;
		s.push_back(j);
	}
}

void Shader::Load(const nlohmann::json& inJSON)
{
	fm::Resource::Load(inJSON);

	for (const auto& subshader : inJSON["subShaders"])
	{
		fm::SHADER_KIND kind = (fm::SHADER_KIND)subshader["kind"];
		ShaderID id = uint64_t(kind) | (uint64_t(_currentID) << 32);

		SubShader sub(fm::FilePath(_path).ToSub(std::to_string(kind)), id);
		sub.Load(subshader["data"]);

		_subShaders.emplace((fm::SHADER_KIND)subshader["kind"], sub);
	}
}

void Shader::AddSubShader(SHADER_KIND inKind, const SubShader::Reflections& inReflection)
{
	ShaderID id = uint64_t(inKind) | (uint64_t(_currentID) << 32);
	SubShader sub(fm::FilePath(_path).ToSub(std::to_string(inKind)), id);
	sub.SetReflection(inReflection);
	_subShaders.emplace(inKind, sub);
}

std::optional<SubShader> Shader::GetSubShader(SHADER_KIND inKind) const
{
	auto it = _subShaders.find(inKind);
	if (it != _subShaders.end())
		return it->second;

	return std::nullopt;
}


std::vector<SHADER_KIND> Shader::GetPossibleShaderKind() const
{
	std::vector<SHADER_KIND> kinds;
	for (const auto& [k, _] : _subShaders)
	{
		kinds.push_back(k);
	}

	return kinds;
}


Shader::~Shader()
{
}


SHADER_KIND Shader::ConvertStringsToShaderKind(const std::vector<std::string>& inStrings)
{
	int kind = SHADER_KIND::PLAIN;
	for (const auto& s : inStrings)
	{
		if (s == "texture")
		{
			kind |= (int)SHADER_KIND::TEXTURE;
		}
	}
	return (SHADER_KIND)kind;
}

