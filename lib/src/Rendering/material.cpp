#include <Rendering/Shader.h>

#include "Rendering/material.hpp"
#include <nlohmann/json.hpp>
#include "Resource/ResourcesManager.h"
#include "Rendering/Texture.h"
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
	mat._materialPropertiesInfos = _materialPropertiesInfos;
	mat._materialTexturesInfos = _materialTexturesInfos;


	memcpy(mat._buffer, _buffer, _bufferSize);
	return mat;
}


void Material::From(const fm::Material& inMaterial)
{
	_shaderPath = inMaterial._shaderPath;
	_properties = inMaterial._properties;
	_uniforms = inMaterial._uniforms;
	_bufferSize = inMaterial._bufferSize;
	_buffer = (unsigned char*)calloc(_bufferSize, sizeof(unsigned char));
	_materialPropertiesInfos = inMaterial._materialPropertiesInfos;
	_materialTexturesInfos = inMaterial._materialTexturesInfos;

	if (_buffer == nullptr)
	{
		assert(false);
		return;
	}

	if (inMaterial._buffer == nullptr && !_properties.empty() )
	{
		_UpdateInternalBuffWithProperties();
	}
	else
	{
		memcpy(_buffer, inMaterial._buffer, _bufferSize);
	}
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
	else if (type == fm::ValuesType::VALUE_VECTOR4_UINTEGER)
		valueJSON["value"] = inValue.getVector4ui();
	else if (type == fm::ValuesType::VALUE_TEXTURE)
	{
		auto texture = inValue.getTexture();
		if (texture != nullptr)
		{
			valueJSON["value"] = fm::FileSystem::ConvertPathToFileSystem(texture->GetPath());
		}

	}
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
		fm::Color c = (fm::Color)valueJSON["value"];
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
	else if (type == fm::ValuesType::VALUE_VECTOR4_UINTEGER)
	{
		fm::math::vec4ui c = valueJSON["value"];
		outValue = fm::MaterialValue(c);
	}
	else if(type == fm::ValuesType::VALUE_TEXTURE)
	{
		fm::FilePath path((std::string)valueJSON["value"]);
		outValue = std::make_shared<fm::Texture>(path);
	}
}


void Material::Load(const nlohmann::json& inJSON)
{
	if (inJSON.is_null())
		return;
	fm::Resource::Load(inJSON);
	nlohmann::json params = inJSON["params"];

	_name = params["name"];

	fm::FilePath path = fm::FilePath((std::string)params["shader"]);
	_shaderPath = path;
	_BuildMaterialPropertiesInfo();
	_materialTexturesInfos = GetMaterialBufferInfo_Textures(GRAPHIC_API::OPENGL);
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
			if (auto it2 = _materialPropertiesInfos.find(name); it2 != _materialPropertiesInfos.end())
			{
				UpdateProperty(name, value, it2->second.offset);
			}
			else
			{
				_properties[name] = value;
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
	auto shader = fm::ResourcesManager::get().getResource<fm::Shader>(_shaderPath);
	if (shader == nullptr)
		return std::nullopt;

	return shader->GetSubShader(_shaderkind);
}


void Material::Save() const
{
	nlohmann::json json;
	fm::File file(_path);
	file.CreateFile();
	Save(json);

	file.SetContent(json.dump());
}


void Material::SetShaderPath(const fm::FilePath& inPath, fm::SHADER_KIND inKind)
{
	bool hasChanged = _shaderPath != inPath || _shaderkind != inKind;
	
	_shaderPath = inPath;
	_shaderkind = inKind;

	if (hasChanged)
	{
		_BuildMaterialPropertiesInfo();
	}

}

MaterialKind Material::GetMaterialKind() const
{
	const bool isFromInternal = _shaderPath.GetFileSystemID() == fm::LOCATION::INTERNAL_SHADERS_LOCATION;
	if (isFromInternal && _shaderPath.GetName(false) == "default.shader")
		return MaterialKind::STANDARD;
	return MaterialKind::SHADER;
}



template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void Material::UpdateProperty(const std::string& inName, const fm::MaterialValue& inProperty, uint32_t offset)
{
	_properties[inName] =  inProperty;
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
			[&buffer, offset](const auto& arg) {
				const uint32_t size = sizeof(arg);
				memcpy(buffer + offset, &arg, size);
			},
		}, inProperty.GetConstVariant());
	}
}



void _BlockIterator(const fm::SubShader::Reflection& reflection, const std::string& name, uint32_t& offset, const fm::SubShader::Variable& inVariable, std::unordered_map<std::string, MaterialValueInfo> &properties)
{
	const auto block = reflection.blocks.find(inVariable.typeName);

	if (block != reflection.blocks.end())
	{
		for (const auto& variable : block->second.variables)
		{
			if (variable.isBlock)
			{
				_BlockIterator(reflection, "[" + variable.typeName + "]" + variable.name + "." + name, offset, variable, properties);
			}
			else
			{
				MaterialValueInfo value;
				value.name = name + "." + variable.name;
				value.offset = offset;
				value.size = variable.size;
				value.type = variable.type;

				properties.emplace(value.name, value);

				offset += variable.size;

			}
		}
	}
}

void Material::_BuildMaterialPropertiesInfo()
{
	std::unordered_map<std::string, MaterialValueInfo> properties;
	std::optional<fm::SubShader> currentSubShader = GetSubShader();

	if (currentSubShader.has_value())
	{
		const auto reflection = currentSubShader->GetReflection(GRAPHIC_API::OPENGL);
		for (const auto& [name, uniform] : reflection.uniforms)
		{
			auto block = reflection.blocks.find(name);
			uint32_t offset = 0;
			if (block != reflection.blocks.end())
			{
				for (const auto& variable : block->second.variables)
				{
					if (variable.isBlock)
					{
						_BlockIterator(reflection, uniform.name + "[" + variable.typeName + "]" + variable.name, offset, variable, properties);
					}
					else
					{
						MaterialValueInfo value;
						value.name = name + "." + variable.name;
						value.offset = offset;
						value.size = variable.size;
						value.type = variable.type;

						properties.emplace(value.name, value);

						offset += variable.size;
					}
				}
			}
		}
	}
	_materialPropertiesInfos = properties;
}


std::unordered_map<std::string, MaterialValueInfo> Material::GetMaterialPropertiesInfo() const
{
	return _materialPropertiesInfos;
}


void Material::_UpdateInternalBuffWithProperties()
{
	std::unordered_map<std::string, MaterialValueInfo> properties = GetMaterialPropertiesInfo();

	for (const auto& [name, value] : GetProperties())
	{
		if (auto info = properties.find(name); info != properties.end())
		{
			UpdateProperty(name, value, info->second.offset);
		}
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

std::vector<fm::SubShader::Uniform> Material::_GetReflectionTextures(GRAPHIC_API inAPI) const
{
	std::vector<fm::SubShader::Uniform> uniforms;
	auto currentSubShader = GetSubShader();
	if (currentSubShader.has_value())
	{
		auto reflection = currentSubShader->GetReflection(inAPI);
		for (const auto& [name, uniform] : reflection.uniforms)
		{
			if (uniform.IsTexture())
			{
				uniforms.emplace_back(uniform);
			}
		}
	}
	return uniforms;
}

bool Material::CanBeModified() const
{
	return !Resource::IsInternal();
}

std::shared_ptr<fm::Material> Material::GetDefaultStandardMaterial()
{
	return fm::ResourcesManager::get().getResource<fm::Material>(fm::FilePath(fm::LOCATION::INTERNAL_MATERIALS_LOCATION, "default.material"));
}

std::vector<Material::MaterialBufferInfo_Texture> Material::GetMaterialBufferInfo_Textures(GRAPHIC_API inAPI) const
{
	MaterialBufferInfo info;

	std::vector<Material::MaterialBufferInfo_Texture> textures;

	for (const auto& uniform : _GetReflectionTextures(inAPI))
	{
		Material::MaterialBufferInfo_Texture texture;
		texture.info.bindingPoint = uniform.binding;
		texture.info.setPoint = uniform.set;
		texture.info.stages = (fm::SubShader::STAGE)uniform.stages;
		if (auto it = _properties.find(uniform.name); it != _properties.end())
		{
			auto value = it->second;
			texture.texture = value.getTexture();
		}
		texture.name = uniform.name;
		textures.push_back(texture);

	}

	std::sort(textures.begin(), textures.end(), [](const auto& inTextureA, const auto& inTextureB) {
		return inTextureA.info.bindingPoint < inTextureB.info.bindingPoint;
	});
	

	return textures;
}


void Material::SetStandardTexture(fm::STANDARD_MATERIAL_PROPERTIES_TEXTURE inTextureKind, const fm::FilePath& inPath)
{
	auto info = GetMaterialPropertiesInfo();
	if (auto it = info.find("MaterialBuffer._properties"); it != info.end())
	{
		if (inTextureKind >= _materialTexturesInfos.size())
		{
			assert(false); //Too many textures
		}

		_properties[_materialTexturesInfos[inTextureKind].name] = fm::MaterialValue(fm::ResourcesManager::get().getResource<fm::Texture>(inPath));
		fm::math::vec4ui properties;
		if (auto it2 = _properties.find("MaterialBuffer._properties"); it2 != _properties.end())
		{
			properties = it2->second.getVector4ui();
		}
		properties.x |= (unsigned int)(1 << inTextureKind);
		UpdateProperty("MaterialBuffer._properties", properties, it->second.offset);
	}
}
std::shared_ptr<fm::Texture> Material::GetStandardTexture(fm::STANDARD_MATERIAL_PROPERTIES_TEXTURE inTextureKind) const
{
	if (inTextureKind >= _materialTexturesInfos.size())
	{
		assert(false); //Too many textures
	}

	if (auto it = _properties.find(_materialTexturesInfos[inTextureKind].name); it != _properties.end())
	{
		return it->second.getTexture();
	}
	return nullptr;
}


bool Material::HasStandardTexture(fm::STANDARD_MATERIAL_PROPERTIES_TEXTURE inTextureKind) const
{
	if (inTextureKind >= _materialTexturesInfos.size())
	{
		assert(false); //Too many textures
	}
	return _properties.contains(_materialTexturesInfos[inTextureKind].name);
}


Material::MaterialBufferInfo_Buffer Material::GetMaterialBufferInfo_Buffer(GRAPHIC_API inAPI) const
{
	MaterialBufferInfo info;
	auto uniform = _GetReflectionUniform("MaterialBuffer", inAPI);
	if (uniform.has_value())
	{
		info.bindingPoint = uniform->binding;
		info.setPoint = uniform->set;
		info.stages = (fm::SubShader::STAGE)uniform->stages;
	}
	MaterialBufferInfo_Buffer buffer;
	buffer.info = info;
	buffer.buffer = _buffer;
	buffer.bufferSize = _bufferSize;

	return buffer;
}
