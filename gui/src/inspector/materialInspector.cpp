#include "inspector/materialInspector.hpp"
#include <imgui/imgui.h>
#include <Resource/ResourcesManager.h>
#include <Rendering/Shader.h>
#include <Core/Debug.h>
#include <Rendering/material.hpp>
#include "ECS.h"
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Material, fmc::CMaterial)


bool DrawCombo(const std::string &inNameCombo, const std::vector<const char*>& values, std::string &currentItem, size_t *index)
{
    const char* item_current_char = currentItem.c_str();
    std::string nameCombo = inNameCombo;
    if (ImGui::BeginCombo(nameCombo.c_str(), item_current_char, 0))
    {
        for (size_t n = 0; n < values.size(); n++)
        {
            bool is_selected = (item_current_char == values[n]);
            if (ImGui::Selectable(values[n], is_selected))
            {
                item_current_char = values[n];
                *index = n;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
	bool hasChanged = currentItem != std::string(item_current_char);
    currentItem = std::string(item_current_char);
	return hasChanged;
}

void MaterialInspector::_DeInit()
{
	for (auto&& type : _typesMaterial)
	{
		delete type;
	}

	_typesMaterial.clear();
}

std::string MaterialInspector::_ValueTypeToName(fm::ValuesType inValueType)
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


void MaterialInspector::_Init()
{
    fm::Debug::get().LogError("InitMaterial");
    std::map<std::string, std::shared_ptr<fm::Resource>> resources = fm::ResourcesManager::get().getAll<fm::Shader>();
    for(auto &r : resources)
    {
        fm::Shader *s = static_cast<fm::Shader*>(r.second.get());
        _valuesShader.push_back(s->GetName().c_str());
    }

    for(size_t i = 0; i < (size_t)fm::ValuesType::VALUE_LAST; ++i)
    {
		std::string name = _ValueTypeToName((fm::ValuesType)i);
		char* c = new char[name.size() + 1];
		strcpy(c, name.c_str());
        _typesMaterial.emplace_back(c);
    }

}

void MaterialInspector::Draw(bool *value, const Entity& e)
{
	std::string name = _target->GetName() + "##" + std::to_string(e.id().index());

    if(ImGui::CollapsingHeader(name.c_str(), value))
    {
        for(auto &m :_target->GetAllMaterials())
        {
            std::string materialName = m->GetName() + "##Material";

            if (ImGui::TreeNode(materialName.c_str()))
            {
                size_t j = 0;
                std::vector<fm::MaterialProperty>::const_iterator materialValue = m->getValues().begin();
				std::string shaderName = m->GetShader()->GetName();

                for (; materialValue != m->getValues().end(); )
                {
#if 1
					fm::MaterialProperty currentProperty = *materialValue;

                    fm::ValuesType type = currentProperty.materialValue.getType();
                    const fm::ValuesType ctype = type;
					std::string currentTypeName = _ValueTypeToName(type);

                    static char nameType[256];
                    memcpy(nameType, currentProperty.name.c_str(), currentProperty.name.length());
                    {
						size_t t = (size_t)type;
                        DrawCombo("Type##"+ shaderName + currentProperty.name + std::to_string(j), _typesMaterial, currentTypeName, &t);
						type = (fm::ValuesType)t;
                        std::string nameTextInput = std::string(nameType) + "##Text"+ shaderName + currentProperty.name + std::to_string(j);
                        if(ImGui::InputText(nameTextInput.c_str(), nameType, IM_ARRAYSIZE(nameType), ImGuiInputTextFlags_EnterReturnsTrue))
                        {
							currentProperty.name.assign(nameType);
                        }
                    }

                    std::string name = std::string(materialValue->name) + " "+ _ValueTypeToName(type) + "##" + shaderName + materialValue->name;
                    name += std::to_string(j);

                    if(type == fm::ValuesType::VALUE_COLOR)
                    {
                        fm::Color c = currentProperty.materialValue.getColor();
                        ImGui::PushID(name.c_str());
                        ImGui::ColorEdit3("##", &c.r);
                        ImGui::PopID();

						currentProperty.materialValue = c;
                    }
                    else if(type == fm::ValuesType::VALUE_INT)
                    {

                        int c = currentProperty.materialValue.getInt();
                        ImGui::PushID(name.c_str());
                        ImGui::InputInt("##", &c);
                        ImGui::PopID();

						currentProperty.materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_FLOAT)
                    {

                        float c = currentProperty.materialValue.getInt();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat("##", &c);
                        ImGui::PopID();
						currentProperty.materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
                    {

                        fm::math::vec2 c = currentProperty.materialValue.getVector2();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat2("##", &c.x);
                        ImGui::PopID();

						currentProperty.materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
                    {

                        fm::math::vec3 c = currentProperty.materialValue.getVector3();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat3("##", &c.x);
                        ImGui::PopID();

						currentProperty.materialValue = c;
                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
                    {
                        fm::math::vec4 c = currentProperty.materialValue.getVector4();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat4("##", &c.x);
                        ImGui::PopID();

						currentProperty.materialValue = c;
                    }

					m->UpdateProperty(j, currentProperty);
					
					j++;
                    materialValue++;

                    ImGui::Separator();
#endif
                }

                if(ImGui::Button("Add"))
                {
                    m->setValue("NEW", (int)0);
                }

                std::string currentItem = shaderName.c_str();
                size_t index = 0;
				if (DrawCombo("Shader##" + m->GetName(), _valuesShader, currentItem, &index))
				{
					m->SetShader(fm::ResourcesManager::get().getResource<fm::Shader>(currentItem));
				}

                ImGui::TreePop();
            }
        }
    }
}

void MaterialInspector::RemoveComponent(const Entity& inEntity)
{
	EntityManager::get().removeComponent<fmc::CMaterial>(inEntity.id());
}