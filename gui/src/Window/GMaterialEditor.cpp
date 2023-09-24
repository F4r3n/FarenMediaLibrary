#include "GMaterialEditor.h"
#include "Components/CTransform.h"
#include "Core/GameObject.h"
#include <memory>
#include "Components/CIdentity.h"
#include "Components/CCamera.h"
#include "Core/application.h"
#include "Window.h"
#include <Resource/ResourcesManager.h>
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


void GMaterialEditor::DrawMaterialInspector(fm::Material* inMaterial)
{
	if (inMaterial == nullptr)
		return;

	size_t j = 0;
	//std::string shaderName = inMaterial->GetShader()->GetName();

	for (auto& materialProperty : inMaterial->GetProperties())
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
			ImGui::ColorEdit3("##", &c.r);
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

		inMaterial->UpdateProperty(j, materialProperty);

		j++;

		ImGui::Separator();
	}
}


void GMaterialEditor::_Update(float dt, Context &inContext)
{
	if (inContext.currentPathSelected.has_value())
	{
		_material = fm::ResourcesManager::get().getResource<fm::Material>(inContext.currentPathSelected.value());
	}
}


void GMaterialEditor::CustomDraw()
{
	if (auto material = _material.lock())
	{
		DrawMaterialInspector(material.get());
	}
}


GMaterialEditor::~GMaterialEditor()
{
}
