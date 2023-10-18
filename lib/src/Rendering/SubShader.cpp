#include "SubShader.hpp"

namespace fm {
	void to_json(nlohmann::json& j, const SubShader::Uniform& p) {
		j["name"] = p.name;
		j["binding"] = p.binding;
		j["set"] = p.set;
		j["type"] = p.type;
	}

	void to_json(nlohmann::json& j, const SubShader::Reflection& p) {
		nlohmann::json& uniforms = j["uniforms"];
		for (const auto& [name, uniform] : p.uniforms)
		{
			uniforms[name] = uniform;
		}
	}
	void from_json(const nlohmann::json& j, SubShader::Uniform& p) {
		p.name = j["name"];
		p.binding = j["binding"];
		p.set = j["set"];
		p.type = j["type"];
	}

	void from_json(const nlohmann::json& j, SubShader::Reflection& p) {
		for (const auto& uniform : j["uniforms"])
		{
			SubShader::Uniform u = uniform;

			p.uniforms.emplace(u.name, u);
		}
	}

}

using namespace fm;

SubShader::SubShader(const fm::FilePath& inPath, ShaderID inID)
{
	_path = inPath;
	_ID = inID;
}

void SubShader::SetReflection(const Reflections& inReflection)
{
	_reflections = inReflection;
}

void SubShader::Save(nlohmann::json& outJSON) const
{
	outJSON["reflection"] = this->_reflections;
	outJSON["path"] = fm::FileSystem::ConvertPathToFileSystem(_path);
}

void SubShader::Load(const nlohmann::json& inJSON)
{
	this->_reflections = inJSON["reflection"];
	this->_path = fm::FilePath(std::string(inJSON["path"]));
}