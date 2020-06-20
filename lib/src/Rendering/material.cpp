#include <Rendering/Shader.h>

#include "Rendering/material.hpp"
#include <nlohmann/json.hpp>
#include "Resource/ResourcesManager.h"
#include <fstream>

using namespace fm;

Material::Material(const fm::FilePath& inFilePath)
	:_name(inFilePath.GetName(true)),
	Resource(inFilePath)
{
    setValue("mainColor", fm::Color(1,1,1,1));
}

void Material::Save() const
{
	nlohmann::json json;
	json["name"] = _name;
	json["shaderName"] = _shader != nullptr ? _shader->GetName() : "";

	nlohmann::json valuesJSON;
	for (auto&& value : _properties.GetValues())
	{
		nlohmann::json valueJSON;
		fm::ValuesType type = value.materialValue.getType();
		
		valueJSON["name"] = value.name;
		valueJSON["type"] = type;
		if (type == fm::ValuesType::VALUE_INT)
			valueJSON["value"] = value.materialValue.getInt();
		else if (type == fm::ValuesType::VALUE_COLOR)
			valueJSON["value"] = value.materialValue.getColor();
		else if (type == fm::ValuesType::VALUE_FLOAT)
			valueJSON["value"] = value.materialValue.getFloat();
		else if (type == fm::ValuesType::VALUE_MATRIX_FLOAT)
			valueJSON["value"] = value.materialValue.getMatrix();
		//else if(type == fm::ValuesType::VALUE_TEXTURE)
		//    valueJSON["value"] = value.materialValue.getTexture();
		else if (type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
			valueJSON["value"] = value.materialValue.getVector2();
		else if (type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
			valueJSON["value"] = value.materialValue.getVector3();
		else if (type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
			valueJSON["value"] = value.materialValue.getVector4();
		valuesJSON.push_back(valueJSON);
	}
	json["materialValues"] = valuesJSON;

	std::ofstream o(_path.GetPath());
	o  << json << std::endl;

}

void Material::Load()
{
	std::ifstream i(_path.GetPath());
	nlohmann::json json;
	i >> json;


	_name = json["name"];
	std::string shaderName = json["shaderName"];
	_shader = fm::ResourcesManager::get().getResource<fm::Shader>(shaderName);
	nlohmann::json values = json["materialValues"];
	for (nlohmann::json::iterator it = values.begin(); it != values.end(); ++it)
	{
		nlohmann::json v = *it;
		fm::ValuesType type = v["type"];
		if (type == fm::ValuesType::VALUE_INT)
			setValue(v["name"], (int)v["value"]);
		else if (type == fm::ValuesType::VALUE_COLOR)
		{
			fm::Color c = v["value"];
			setValue(v["name"], c);
		}
		else if (type == fm::ValuesType::VALUE_FLOAT)
			setValue(v["name"], (float)v["value"]);
		else if (type == fm::ValuesType::VALUE_MATRIX_FLOAT)
		{
			fm::math::mat mat = v["value"];
			setValue(v["name"], mat);
		}
		//else if(type == fm::ValuesType::VALUE_TEXTURE)
		//{
		//    fm::TextureMat t = v["value"];
		//    setValue(v["name"], t);
		//}
		else if (type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
		{
			fm::math::vec2 c = v["value"];
			setValue(v["name"], c);
		}
		else if (type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
		{
			fm::math::vec3 c = v["value"];
			setValue(v["name"], c);
		}
		else if (type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
		{
			fm::math::vec4 c = v["value"];
			setValue(v["name"], c);
		}
	}
}


const std::vector<MaterialProperty> &Material::getValues() const
{
    return _properties.GetValues();
}

bool Material::IsReady() const
{
	return _shader != nullptr && _shader->IsReady();
}

void Material::Compile()
{
	if (IsReady())
	{
		_shader->compile();
	}
}



