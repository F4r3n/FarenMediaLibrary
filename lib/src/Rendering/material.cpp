#include <Rendering/Shader.h>

#include "Rendering/material.hpp"
#include <nlohmann/json.hpp>
#include "Resource/ResourcesManager.h"
#include <fstream>

using namespace fm;

Material::Material(const fm::FilePath& inFilePath)
	:Resource(inFilePath), _name(inFilePath.GetName(true))
{
	_ID++;
	_currentID = _ID;
}

Material Material::Clone() const
{
	fm::FilePath p(_name);
	Material mat(p);
	mat._shaderPath = _shaderPath;
	mat._properties = _properties;
	mat._uniforms = _uniforms;
	mat._bufferSize = _bufferSize;
	memcpy(mat._buffer, _buffer, _bufferSize);
	return mat;
}

void Material::_FillJSONValue(nlohmann::json& valueJSON, const std::string& inName, const fm::MaterialValue& inValue) const
{
	fm::ValuesType type = inValue.getType();

	valueJSON["name"] = inName;
	valueJSON["type"] = type;
	if (type == fm::ValuesType::VALUE_INT)
		valueJSON["value"] = inValue.getInt();
	else if (type == fm::ValuesType::VALUE_COLOR)
		valueJSON["value"] = inValue.getColor();
	else if (type == fm::ValuesType::VALUE_FLOAT)
		valueJSON["value"] = inValue.getFloat();
	else if (type == fm::ValuesType::VALUE_MATRIX_FLOAT)
		valueJSON["value"] = inValue.getMatrix();
	else if (type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
		valueJSON["value"] = inValue.getVector2();
	else if (type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
		valueJSON["value"] = inValue.getVector3();
	else if (type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
		valueJSON["value"] = inValue.getVector4();
}

void Material::Save(nlohmann::json& outJSON) const
{
	fm::Resource::Save(outJSON);

	nlohmann::json params;
	params["name"] = _name;
	std::string shaderPath = fm::FileSystem::ConvertPathToFileSystem(_shaderPath);
	params["shader"] = shaderPath;
	params["shaderKind"] = _shaderkind;
	{
		nlohmann::json valuesJSON;
		for (auto& [name, value] : _properties)
		{
			nlohmann::json valueJSON;
			_FillJSONValue(valueJSON, name, value);

			valuesJSON.push_back(valueJSON);
		}
		params["materialValues"]["properties"] = valuesJSON;
	}

	{
		nlohmann::json valuesJSON;
		for (auto& [name, value] : _uniforms)
		{
			nlohmann::json valueJSON;
			_FillJSONValue(valueJSON, name, value);

			valuesJSON.push_back(valueJSON);
		}
		params["materialValues"]["uniforms"] = valuesJSON;
	}

	outJSON["params"] = params;

}


void Material::_GetJSONValue(const nlohmann::json& valueJSON, std::string& outName, fm::MaterialValue& outValue) const
{
	fm::ValuesType type = valueJSON["type"];
	outName = valueJSON["name"];

	if (type == fm::ValuesType::VALUE_INT)
	{
		outValue = fm::MaterialValue((int)valueJSON["value"]);
	}
	else if (type == fm::ValuesType::VALUE_COLOR)
	{
		fm::Color c = valueJSON["value"];
		outValue = fm::MaterialValue(c);
	}
	else if (type == fm::ValuesType::VALUE_FLOAT)
	{
		outValue = fm::MaterialValue((float)valueJSON["value"]);
	}
	else if (type == fm::ValuesType::VALUE_MATRIX_FLOAT)
	{
		fm::math::mat c = valueJSON["value"];
		outValue = fm::MaterialValue(c);
	}
	else if (type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
	{
		fm::math::vec2 c = valueJSON["value"];
		outValue = fm::MaterialValue(c);
	}
	else if (type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
	{
		fm::math::vec3 c = valueJSON["value"];
		outValue = fm::MaterialValue(c);
	}
	else if (type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
	{
		fm::math::vec4 c = valueJSON["value"];
		outValue = fm::MaterialValue(c);
	}

	//else if(type == fm::ValuesType::VALUE_TEXTURE)
//{
//    fm::TextureMat t = v["value"];
//    setValue(v["name"], t);
//}
}


void Material::Load(const nlohmann::json& inJSON)
{
	fm::Resource::Load(inJSON);
	nlohmann::json params = inJSON["params"];

	_name = params["name"];

	fm::FilePath path = fm::FilePath((std::string)params["shader"]);
	_shaderPath = path;

	if (params.contains("shaderKind"))
	{
		_shaderkind = params["shaderKind"];
	}
	{
		nlohmann::json values = params["materialValues"]["properties"];
		for (nlohmann::json::iterator it = values.begin(); it != values.end(); ++it)
		{
			nlohmann::json v = *it;
			std::string name;
			fm::MaterialValue value;
			_GetJSONValue(v, name, value);
			//get offset
			uint32_t offset = 0;
			if (_GetOffsetFromReflection(name, offset))
			{
				UpdateProperty(name, value, offset);
			}
		}
	}

	{
		nlohmann::json values = params["materialValues"]["uniforms"];
		for (nlohmann::json::iterator it = values.begin(); it != values.end(); ++it)
		{
			nlohmann::json v = *it;
			std::string name;
			fm::MaterialValue value;
			_GetJSONValue(v, name, value);
			SetUniformValue(name, value);
		}
	}

}

bool _BlockIterator(const fm::SubShader::Reflection& reflection, const fm::SubShader::Variable& inVariable, const std::string &nameToFind, size_t inNameIndex,
	const std::string& name, uint32_t& offset)
{
	std::string blockName;
	size_t i = inNameIndex;
	for (; i < nameToFind.size(); ++i)
	{
		const char c = nameToFind[i];
		if (c == '[')
			continue;
		if (c == ']')
		{
			break;
		}
		else
		{
			blockName += c;
		}
	}
	i++;
	std::string currentName = name + "[" + blockName + "]" + inVariable.name;

	const auto block = reflection.blocks.find(blockName);

	if (block != reflection.blocks.end())
	{
		for (const auto& variable : block->second.variables)
		{
			std::string n = currentName + "." + variable.name;
			if (n == nameToFind)
				return true;
			if (variable.isBlock)
			{
				_BlockIterator(reflection, variable, nameToFind, i, n + "." + name, offset);
			}
			else
			{
				offset += variable.size;
			}
		}
	}
	return false;
}

bool Material::_GetOffsetFromReflection(const std::string& inName, uint32_t& offset) const
{
	std::string blockName;
	size_t i = 0;
	for (i = 0; i < inName.size(); ++i)
	{
		const char c = inName[i];
		if (c == '[')
		{
			break;
		}
		else
		{
			blockName += c;
		}
	}

	auto subShader = GetSubShader();
	if (!subShader.has_value())
		return false;


	auto reflection = subShader->GetReflection(GRAPHIC_API::OPENGL);

	auto it = reflection.blocks.find(blockName);
	bool ok = false;
	if (it != reflection.blocks.end())
	{
		for (const auto& variable : it->second.variables)
		{
			uint32_t currentOffset = 0;
			ok = _BlockIterator(reflection, variable, inName, i, blockName, currentOffset);
			if (ok)
			{
				ok = true;
				break;
			}
			offset += currentOffset;
		}
	}
	return ok;
}


std::optional<fm::SubShader> Material::GetSubShader() const
{
	auto shader = fm::ResourcesManager::get().getResource<fm::Shader>(GetShaderPath());
	if (shader == nullptr)
		return std::nullopt;

	return shader->GetSubShader(_shaderkind);
}


void Material::Save() const
{
	nlohmann::json json;
	Save(json);

	fm::File file(_path);
	file.SetContent(json.dump());
}


void Material::SetShaderPath(const fm::FilePath& inPath)
{
	_shaderPath = inPath;
}

MaterialKind Material::GetMaterialKind() const
{
	const bool isFromInternal = _shaderPath.GetFileSystemID() == fm::LOCATION::INTERNAL_SHADERS_LOCATION;
	if (isFromInternal && _shaderPath.GetName(false) == "default.shader")
		return MaterialKind::STANDARD;
	return MaterialKind::SHADER;
}

void Material::SetShaderKind(fm::SHADER_KIND inKind)
{
	_shaderkind = inKind;
}

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void Material::UpdateProperty(const std::string& inName, fm::MaterialValue& inProperty, uint32_t offset)
{
	GetProperties().UpdateProperty(inName, inProperty);
	if (_buffer == nullptr)
	{
		auto block = _GetReflectionBlock("MaterialBuffer", GRAPHIC_API::OPENGL);
		if (block.has_value())
		{
			_bufferSize = block->size;
			_buffer = (unsigned char*)calloc(_bufferSize, sizeof(unsigned char));
		}
	}

	if (_buffer != nullptr)
	{
		Resource::Touch();
		unsigned char* buffer = _buffer;
		std::visit(overloaded{
			[&buffer, offset](auto& arg) {
				const uint32_t size = sizeof(arg);
				memcpy(buffer + offset, &arg, size);
			},
		}, inProperty.GetVariant());
	}
}
std::optional<fm::SubShader::Block>	Material::_GetReflectionBlock(const std::string& inName, GRAPHIC_API inAPI) const
{
	auto currentSubShader = GetSubShader();
	if (currentSubShader.has_value())
	{
		auto reflection = currentSubShader->GetReflection(inAPI);
		auto it = reflection.blocks.find(inName);
		if (it != reflection.blocks.end())
		{
			return it->second;
		}
	}
	return std::nullopt;
}

std::optional<fm::SubShader::Uniform> Material::_GetReflectionUniform(const std::string& inName, GRAPHIC_API inAPI) const
{
	auto currentSubShader = GetSubShader();
	if (currentSubShader.has_value())
	{
		auto reflection = currentSubShader->GetReflection(inAPI);
		auto it = reflection.uniforms.find(inName);
		if (it != reflection.uniforms.end())
		{
			return it->second;
		}
	}
	return std::nullopt;
}



Material::MaterialBufferInfo Material::GetMaterialBufferInfo(GRAPHIC_API inAPI) const
{
	MaterialBufferInfo info;
	auto uniform = _GetReflectionUniform("MaterialBuffer", inAPI);
	if (uniform.has_value())
	{
		info.bindingPoint = uniform->binding;
		info.setPoint = uniform->set;
		info.stages = (fm::SubShader::STAGE)uniform->stages;
	}
	info.buffer = _buffer;
	info.bufferSize = _bufferSize;

	return info;
}
