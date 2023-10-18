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
	_ID++;
	_currentID = _ID;
}

void Material::Save(nlohmann::json& outJSON) const
{
	fm::Resource::Save(outJSON);

	nlohmann::json params;
	params["name"] = _name;
	std::string shaderPath = fm::FileSystem::ConvertPathToFileSystem(_shaderPath);
	params["shader"] = shaderPath;
	params["kind"] = _kind;
	nlohmann::json valuesJSON;
	for (auto& value : _properties)
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
		else if (type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
			valueJSON["value"] = value.materialValue.getVector2();
		else if (type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
			valueJSON["value"] = value.materialValue.getVector3();
		else if (type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
			valueJSON["value"] = value.materialValue.getVector4();
				//else if(type == fm::ValuesType::VALUE_TEXTURE)
		//    valueJSON["value"] = value.materialValue.getTexture();
		valuesJSON.push_back(valueJSON);
	}
	params["materialValues"] = valuesJSON;
	outJSON["params"] = params;

}

void Material::Load(const nlohmann::json& inJSON)
{
	fm::Resource::Load(inJSON);
	nlohmann::json params = inJSON["params"];

	_name = params["name"];

	fm::FilePath path = fm::FilePath((std::string)params["shader"]);
	_shaderPath = path;
	if (params.contains("kind"))
	{
		_kind = params["kind"];
	}

	//_shader->Load();
	nlohmann::json values = params["materialValues"];
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
		//else if(type == fm::ValuesType::VALUE_TEXTURE)
//{
//    fm::TextureMat t = v["value"];
//    setValue(v["name"], t);
//}
	}
}

std::optional<fm::SubShader> Material::GetSubShader()
{
	auto shader = fm::ResourcesManager::get().getResource<fm::Shader>(GetShaderPath());
	if (shader == nullptr)
		return std::nullopt;

	return shader->GetSubShader(_kind);
}


//bool Material::IsReady() const
//{
//	//return _shader != nullptr && _shader->IsReady();
//}
//
//void Material::Compile()
//{
//	//if (IsReady())
//	//{
//	//	_shader->compile();
//	//}
//}

void Material::Save() const
{
	nlohmann::json json;
	Save(json);

	fm::File file(_path);
	file.SetContent(json.dump());
}


