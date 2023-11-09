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
#include "Rendering/Texture2D.hpp"
#include "Rendering/Texture.h"
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
	case fm::ValuesType::VALUE_VECTOR4_UINTEGER:
		return "Vector4ui";
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
						const std::string currentName = value.info.name;
						std::visit(overloaded{
							[currentName, material, &v](fm::math::vec4& arg) {
								ImGui::PushID(currentName.c_str());
								if (ImGui::ColorEdit4("##", &arg[0], ImGuiColorEditFlags_NoLabel))
								{
									material->UpdateProperty(currentName, v.value, v.info.offset);
								}
								ImGui::PopID();
							},
							[](auto& arg) {},
							}, value.value.GetVariant());

					}

				}
			}

			ImGui::Text("Texture");
			_TextureViewer(material, fm::STANDARD_MATERIAL_PROPERTIES_TEXTURE::TEXTURE_ALBEDO);


		}
		else
		{
			assert(false);
		}

		if (isDisabled) ImGui::EndDisabled();

	}
}

void GMaterialEditor::_TextureViewer(std::shared_ptr<fm::Material> inMaterial, fm::STANDARD_MATERIAL_PROPERTIES_TEXTURE inKind)
{
	auto texture = inMaterial->GetStandardTexture(inKind);
	ImTextureID id = nullptr;

	if (texture != nullptr)
	{
		if (auto it = _textures.find(texture->GetID()); it != _textures.end())
		{
			id = (ImTextureID)intptr_t(it->second->GetID());
		}
		else
		{
			auto texture2D = fm::Texture2D::CreateTexture2D(GRAPHIC_API::OPENGL, texture->GetPath());
			_textures.emplace(texture->GetID(), texture2D);
			id = (ImTextureID)intptr_t(texture2D->GetID());
		}

	}
	//Texture albedo
	if (ImGui::ImageButton("Texture", id, ImVec2(128, 128)))
	{

	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FileNavigator"))
		{
			char* payload_n = (char*)payload->Data;
			std::string data(payload_n);
			fm::FilePath path = fm::FilePath(data);
			inMaterial->SetStandardTexture(inKind, path);
		}

		ImGui::EndDragDropTarget();
	}
}


void GMaterialEditor::_Save(std::shared_ptr<fm::Material> inMaterial)
{
	inMaterial->Save();
}


void GMaterialEditor::_Init(std::shared_ptr<fm::Material> inCurrentMaterial)
{

	_currentShaderPath.clear();
	_currentKind = inCurrentMaterial->GetShaderKind();
	_currentShaderPath = fm::FileSystem::ConvertPathToFileSystem(inCurrentMaterial->GetShaderPath());
	_currentMaterialKind = inCurrentMaterial->GetMaterialKind();
	_properties.clear();
	std::unordered_map<std::string, fm::MaterialValueInfo> properties = inCurrentMaterial->GetMaterialPropertiesInfo();

	auto currentProperties = inCurrentMaterial->GetProperties();

	for (auto& [name, property] : properties)
	{
		if (auto it = currentProperties.find(name); it != currentProperties.end())
		{
			MaterialValueEditor valueEditor;
			valueEditor.info = property;
			valueEditor.value = it->second;

			_properties.emplace(name, valueEditor);
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
	material->SetShaderPath(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "default.shader"), fm::SHADER_KIND::PLAIN);

	nlohmann::json j;
	material->Save(j);
	std::ofstream o(newFilePath.GetPath(), std::ofstream::out);
	o << j << std::endl;
	o.close();

	fm::ResourcesManager::get().load<fm::Material>(newFilePath, material);

	return newFilePath;
}
