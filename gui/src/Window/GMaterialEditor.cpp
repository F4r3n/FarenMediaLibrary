#include "GMaterialEditor.h"
#include "Components/CTransform.h"
#include "Core/GameObject.h"
#include <memory>
#include "Components/CIdentity.h"
#include "Components/CCamera.h"
#include "Core/application.h"
#include "Window.h"
#include <Resource/ResourcesManager.h>
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "Rendering/MaterialValue.h"
#include "Rendering/material.hpp"
#include "Resource/ResourcesManager.h"
#include <variant>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace gui;

GMaterialEditor::GMaterialEditor() : GWindow("Material Editor", true)
{
	_kind = gui::WINDOWS::WIN_MATERIAL_EDITOR;
}

std::string ValueTypeToName(fm::ValuesType inValueType)
{
	switch (inValueType)
	{
	case fm::ValuesType::VALUE_COLOR:
		return "color";
	case fm::ValuesType::VALUE_FLOAT:
		return "float";
	case fm::ValuesType::VALUE_INT:
		return "int";
	case fm::ValuesType::VALUE_MATRIX_FLOAT:
		return "Matrixf";
	case fm::ValuesType::VALUE_TEXTURE:
		return "Texture";
	case fm::ValuesType::VALUE_VECTOR2_FLOAT:
		return "Vector2f";
	case fm::ValuesType::VALUE_VECTOR3_FLOAT:
		return "Vector3f";
	case fm::ValuesType::VALUE_VECTOR4_FLOAT:
		return "Vector4f";
	default:
		break;
	}
	return "";
}

std::vector<std::string> GMaterialEditor::GetValueTypeNames()
{
	std::vector<std::string> names;
	for (size_t i = 0; i < (size_t)fm::ValuesType::VALUE_LAST; ++i)
	{
		std::string name = ValueTypeToName((fm::ValuesType)i);
		names.emplace_back(name);
	}
	return names;
}

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


void GMaterialEditor::DrawMaterialInspector(std::shared_ptr<fm::Material> inMaterial)
{
	if (inMaterial == nullptr)
		return;

	_CheckMaterial(inMaterial);
	if (auto material = _material.lock())
	{
		const bool isDisabled = !material->CanBeModified();
		if (isDisabled) ImGui::BeginDisabled();

		const auto shaderPath = material->GetShaderPath();

		size_t index = _currentMaterialKind == fm::MaterialKind::STANDARD ? 0 : 1;
		if (DrawCombo("Kind", { "Standard", "Shader" }, &index))
		{
			if (index > 0)
			{
				_currentMaterialKind = (fm::MaterialKind)(index);
			}
		}

		if (_currentMaterialKind == fm::MaterialKind::SHADER)
		{
			if (ImGui::InputText("##ShaderPath", &_currentShaderPath, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				material->SetShaderPath(fm::FilePath(_currentShaderPath));
				_Init(material);
			}
		}

		if (!shaderPath.IsValid())
			return;

		auto shader = fm::ResourcesManager::get().getResource<fm::Shader>(material->GetShaderPath());
		if (shader == nullptr)
			return;


		if (material->GetMaterialKind() == fm::MaterialKind::SHADER)
		{
			for (const auto& kind : shader->GetPossibleShaderKind())
			{
				auto subShader = shader->GetSubShader(kind);
				if (!subShader.has_value())
					continue;

				auto reflection = subShader->GetReflection(GRAPHIC_API::OPENGL);
				for (const auto& [name, uniform] : reflection.uniforms)
				{
					ImGui::Text("%s", name.c_str());
					for (const auto& variable : uniform.variables)
					{
						ImGui::Text("%s", variable.name.c_str());

						if (!variable.isBlock)
						{
							fm::ValuesType type = variable.type;
						}
					}
				}
			}
		}
		else if (material->GetMaterialKind() == fm::MaterialKind::STANDARD)
		{
			if ((_currentKind & fm::SHADER_KIND::PLAIN) == fm::SHADER_KIND::PLAIN)
			{
				for (auto& [name, value] : _properties)
				{
					auto& v = value;
					if (name.starts_with("MaterialBuffer[PLAIN]"))
					{
						const std::string currentName = value.name;
						std::visit(overloaded{
							[currentName, material, &v](fm::math::vec4 &arg) {
								ImGui::PushID(currentName.c_str());
								if (ImGui::ColorEdit4("##", &arg[0], ImGuiColorEditFlags_NoLabel))
								{
									material->UpdateProperty(currentName, v.value, v.offset);
								}
								ImGui::PopID();
							},
							[](auto &arg) {  },
							}, value.value.GetVariant());

					}
				}
			}

			if (ImGui::Checkbox("Texture", &_textureCheckBox))
			{
				if (_textureCheckBox)
				{
					_currentKind = (fm::SHADER_KIND)(_currentKind | fm::SHADER_KIND::TEXTURE);
				}
				else
				{
					_currentKind = (fm::SHADER_KIND)(_currentKind & ~fm::SHADER_KIND::TEXTURE);
				}
				material->SetShaderKind(_currentKind);
			}
			if ((_currentKind & fm::SHADER_KIND::TEXTURE) == fm::SHADER_KIND::TEXTURE)
			{
				ImGui::Text("%s", "Texture");

				//ImGui::PushID("MainColor");
				//ImGui::ColorEdit3("##", &c.r, ImGuiColorEditFlags_NoLabel);
				//ImGui::PopID();
			}
		}
		else
		{
			assert(false);
		}

		if (isDisabled) ImGui::EndDisabled();

		/*
		for (auto& materialProperty : inMaterial->GetUniforms())
		{
			fm::ValuesType type = materialProperty.materialValue.getType();
			const fm::ValuesType ctype = type;
			std::string currentTypeName = ValueTypeToName(type);

			static char nameType[256];
			static std::vector<std::string> valueTypesNames = GetValueTypeNames();
			memcpy(nameType, materialProperty.name.c_str(), materialProperty.name.length());
			{
				size_t t = (size_t)type;
				DrawCombo("Type##" + materialProperty.name + std::to_string(j), valueTypesNames, currentTypeName, &t);
				type = (fm::ValuesType)t;
				std::string nameTextInput = std::string(nameType) + "##Text" + materialProperty.name + std::to_string(j);
				if (ImGui::InputText(nameTextInput.c_str(), nameType, IM_ARRAYSIZE(nameType), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					materialProperty.name = std::string(nameType);
				}
			}

			std::string name = std::string(materialProperty.name) + " " + ValueTypeToName(type) + "##" + materialProperty.name;
			name += std::to_string(j);

			if (type == fm::ValuesType::VALUE_COLOR)
			{
				fm::Color c = materialProperty.materialValue.getColor();
				ImGui::PushID(name.c_str());
				ImGui::ColorEdit3("##", &c.r, ImGuiColorEditFlags_NoLabel);
				ImGui::PopID();

				materialProperty.materialValue = c;
			}
			else if (type == fm::ValuesType::VALUE_INT)
			{
				int c = materialProperty.materialValue.getInt();
				ImGui::PushID(name.c_str());
				ImGui::InputInt("##", &c);
				ImGui::PopID();

				materialProperty.materialValue = c;

			}
			else if (type == fm::ValuesType::VALUE_FLOAT)
			{
				float c = materialProperty.materialValue.getInt();
				ImGui::PushID(name.c_str());
				ImGui::InputFloat("##", &c);
				ImGui::PopID();
				materialProperty.materialValue = c;

			}
			else if (type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
			{
				fm::math::vec2 c = materialProperty.materialValue.getVector2();
				ImGui::PushID(name.c_str());
				ImGui::InputFloat2("##", &c.x);
				ImGui::PopID();

				materialProperty.materialValue = c;

			}
			else if (type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
			{
				fm::math::vec3 c = materialProperty.materialValue.getVector3();
				ImGui::PushID(name.c_str());
				ImGui::InputFloat3("##", &c.x);
				ImGui::PopID();

				materialProperty.materialValue = c;
			}
			else if (type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
			{
				fm::math::vec4 c = materialProperty.materialValue.getVector4();
				ImGui::PushID(name.c_str());
				ImGui::InputFloat4("##", &c.x);
				ImGui::PopID();

				materialProperty.materialValue = c;
			}
			else if (type == fm::ValuesType::VALUE_TEXTURE)
			{
				fm::TextureMat c = materialProperty.materialValue.getTexture();
				ImGui::PushID(name.c_str());
				std::string text = fm::FileSystem::GetRelativePathOfFileSystemPath(c.texture.GetPath());
				if (ImGui::InputText("##", &text))
				{
					c.texture._path = fm::FilePath(fm::LOCATION::USER_LOCATION, text);
				}
				ImGui::PopID();

				materialProperty.materialValue = c;
			}

			inMaterial->UpdateProperty(j, materialProperty);

			j++;
		}*/
	}
}

void GMaterialEditor::_Save(std::shared_ptr<fm::Material> inMaterial)
{
	inMaterial->Save();
}

void GMaterialEditor::_BlockIterator(const fm::SubShader::Reflection& reflection, const std::string& name, uint32_t& offset, const fm::SubShader::Variable& inVariable)
{
	const auto block = reflection.blocks.find(inVariable.typeName);

	if (block != reflection.blocks.end())
	{
		for (const auto& variable : block->second.variables)
		{
			if (variable.isBlock)
			{
				_BlockIterator(reflection, "[" + variable.typeName + "]" + variable.name + "." + name, offset, variable);
			}
			else
			{
				MaterialValueEditor value;
				value.name = name + "." + variable.name;
				value.offset = offset;
				value.size = variable.size;
				value.value.setValue(variable.type);

				_properties.emplace(value.name, value);

				offset += variable.size;

			}
		}
	}
}


void GMaterialEditor::_Init(std::shared_ptr<fm::Material> inCurrentMaterial)
{

	_currentShaderPath.clear();
	_currentKind = inCurrentMaterial->GetShaderKind();
	_currentShaderPath = fm::FileSystem::ConvertPathToFileSystem(inCurrentMaterial->GetShaderPath());
	_textureCheckBox = (_currentKind & fm::SHADER_KIND::TEXTURE) == fm::SHADER_KIND::TEXTURE;
	_currentMaterialKind = inCurrentMaterial->GetMaterialKind();

	_properties.clear();
	std::optional<fm::SubShader> currentSubShader = inCurrentMaterial->GetSubShader();

	if (currentSubShader.has_value())
	{
		const auto reflection = currentSubShader->GetReflection(GRAPHIC_API::OPENGL);
		for (const auto& [name, uniform] : reflection.uniforms)
		{
			if (name.starts_with("_"))
				continue;

			auto block = reflection.blocks.find(name);
			uint32_t offset = 0;
			if (block != reflection.blocks.end())
			{
				for (const auto& variable : block->second.variables)
				{
					if (variable.isBlock)
					{
						_BlockIterator(reflection, uniform.name + "[" + variable.typeName + "]" + variable.name, offset, variable);
					}
				}
			}
		}

		auto currentProperties = inCurrentMaterial->GetProperties();

		for (auto& [name, property] : _properties)
		{
			if (currentProperties.Has(name))
			{
				property.value = currentProperties.Get(name);

			}
		}
	}
}



void GMaterialEditor::_CheckMaterial(std::shared_ptr<fm::Material> inMaterial)
{
	if (inMaterial != nullptr)
	{
		if (_material.expired())
		{
			_material = inMaterial;
			_Init(inMaterial);
		}
		else if (auto currentMat = _material.lock(); inMaterial->GetID() != currentMat->GetID())
		{
			_material = inMaterial;
			_Init(inMaterial);
		}
	}
}


void GMaterialEditor::_Update(float dt, Context& inContext)
{
	if (inContext.currentPathSelected.has_value())
	{
		auto mat = fm::ResourcesManager::get().getResource<fm::Material>(inContext.currentPathSelected.value());
		if (mat != nullptr)
		{
			_CheckMaterial(mat);
		}
	}
}


void GMaterialEditor::CustomDraw()
{
	if (auto material = _material.lock())
	{
		DrawMaterialInspector(material);
		if (material->CanBeModified())
		{
			if (ImGui::Button("Save")) {
				AddEvent([material](gui::GWindow* window, std::optional<gui::Context> context) {
					material.get()->Save();
					});
			}
		}
	}
}


GMaterialEditor::~GMaterialEditor()
{
}


fm::FilePath GMaterialEditor::CreateNewMaterial(const fm::FilePath& inPath)
{
	fm::FilePath path = fm::FilePath(inPath).ToSub("newMaterial.material");
	fm::FilePath newFilePath = fm::File(path).CreateUniqueFile().GetPath();
	std::shared_ptr<fm::Material> material = std::make_shared<fm::Material>(newFilePath);
	material->From(*fm::Material::GetDefaultStandardMaterial());
	material->SetShaderPath(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "default.shader"));
	material->SetShaderKind(fm::SHADER_KIND::PLAIN);

	nlohmann::json j;
	material->Save(j);
	std::ofstream o(newFilePath.GetPath(), std::ofstream::out);
	o << j << std::endl;
	o.close();

	fm::ResourcesManager::get().load<fm::Material>(newFilePath, material);

	return newFilePath;
}
