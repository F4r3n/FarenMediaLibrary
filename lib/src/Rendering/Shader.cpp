
#include "Rendering/Shader.h"
#include <iostream>
#include "Rendering/MaterialValue.h"
#include "Core/Color.h"
#include "Core/FilePath.h"


using namespace fm;

namespace fm {
	void to_json(nlohmann::json& j, const Shader::Uniform& p) {
		j["name"] = p.name;
		j["binding"] = p.binding;
		j["set"] = p.set;
		j["type"] = p.type;
	}

	void to_json(nlohmann::json& j, const Shader::Reflection& p) {
		nlohmann::json &uniforms = j["uniforms"];
		for (const auto& [name, uniform] : p.uniforms)
		{
			uniforms[name] = uniform;
		}
	}
	void from_json(const nlohmann::json& j, Shader::Uniform& p) {
		p.name = j["name"];
		p.binding = j["binding"];
		p.set = j["set"];
		p.type = j["type"];
	}

	void from_json(const nlohmann::json& j, Shader::Reflection& p) {
		for (const auto& uniform : j["uniforms"])
		{
			Shader::Uniform u = uniform;

			p.uniforms.emplace(u.name, u);
		}
	}

}


Shader::Shader() :Resource(fm::FilePath(std::string(""))) {
}

Shader::Shader(const fm::FilePath& inFilePath) : Resource(inFilePath)
{
	_name = inFilePath.GetName(true);
}

void Shader::Save(nlohmann::json& outJSON) const
{
	fm::Resource::Save(outJSON);
	outJSON["reflection"] = this->_reflection;

}

void Shader::Load(const nlohmann::json& inJSON)
{
	fm::Resource::Load(inJSON);
	this->_reflection = inJSON["reflection"];
}



Shader::~Shader()
{
}


