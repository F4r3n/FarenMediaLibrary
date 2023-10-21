#include "SubShader.hpp"

namespace fm {


	void to_json(nlohmann::json& j, const SubShader::Variable& p) {
		j["name"] = p.name;
		j["typeName"] = p.typeName;
		j["type"] = p.type;
		j["isBlock"] = p.isBlock;
		j["size"] = p.size;
		j["offset"] = p.offset;
	}

	void from_json(const nlohmann::json& j, SubShader::Variable& p) {
		p.name = j["name"];
		p.typeName = j["typeName"];
		p.type = j["type"];
		p.isBlock = j["isBlock"];
		p.size = j["size"];
		p.offset = j["offset"];
	}

	void to_json(nlohmann::json& j, const SubShader::Block& p) {
		j["name"] = p.name;
		j["size"] = p.size;
		j["variables"] = (std::vector<fm::SubShader::Variable>)p.variables;
	}

	void from_json(const nlohmann::json& j, SubShader::Block& p) {
		p.name = j["name"];
		p.size = j["size"];
		for (const fm::SubShader::Variable& v : j["variables"])
		{
			p.variables.emplace_back(v);
		}
	}

	void to_json(nlohmann::json& j, const SubShader::Uniform& p) {
		j["name"] = p.name;
		j["binding"] = p.binding;
		j["set"] = p.set;
		j["variables"] = std::vector<SubShader::Variable>(p.variables);
	}

	void from_json(const nlohmann::json& j, SubShader::Uniform& p) {
		p.name = j["name"];
		p.binding = j["binding"];
		p.set = j["set"];
		for (const fm::SubShader::Variable& v : j["variables"])
		{
			p.variables.emplace_back(v);
		}
	}


	void to_json(nlohmann::json& j, const SubShader::Reflection& p) {
		nlohmann::json& uniforms = j["uniforms"];
		for (const auto& [name, uniform] : p.uniforms)
		{
			uniforms[name] = uniform;
		}

		nlohmann::json& blocks = j["blocks"];
		for (const auto& [name, block] : p.blocks)
		{
			blocks[name] = block;
		}
	}

	void from_json(const nlohmann::json& j, SubShader::Reflection& p) {
		for (const auto& uniform : j["uniforms"])
		{
			SubShader::Uniform u = uniform;
			p.uniforms.emplace(u.name, u);
		}

		for (const auto& block : j["blocks"])
		{
			SubShader::Block u = block;
			p.blocks.emplace(u.name, u);
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