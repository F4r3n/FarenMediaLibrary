#include "inspector/MaterialInspector.h"
#include <imgui/imgui.h>
#include <Resource/ResourcesManager.h>
#include <Rendering/Shader.h>
#include <Core/Debug.h>
#include <Rendering/material.hpp>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Material, fmc::CMaterial)

void DrawCombo(const std::string &inNameCombo, const std::vector<const char*>& values, std::string &currentItem, size_t *index)
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

    currentItem = std::string(item_current_char);
}

void MaterialInspector::init()
{
    fm::Debug::get().LogError("InitMaterial");
    std::map<std::string, fm::Resource*> resources = fm::ResourcesManager::get().getAll<fm::Shader>();
    for(auto &r : resources)
    {
        fm::Shader *s = static_cast<fm::Shader*>(r.second);
        valuesShader.push_back(s->GetName().c_str());
    }

    for(size_t i = 0; i < fm::ValuesType::VALUE_LAST; ++i)
    {
        typesMaterial.push_back(fm::MaterialValueNames::ktypeName[i]);
    }

}

void MaterialInspector::draw(bool *value)
{
    static int numberAdded = 0;
    if(ImGui::CollapsingHeader("Material", value))
    {
        size_t i = 0;

        for(auto &m :target->GetAllMaterials())
        {
            std::string materialName = m->GetID() + "##Material";

            if (ImGui::TreeNode(materialName.c_str()))
            {
                size_t j = 0;
                std::vector<fm::MaterialProperty>::iterator materialValue = m->getValues().begin();
                for (; materialValue != m->getValues().end(); )
                {

                    size_t type =  (size_t)materialValue->materialValue.getType();
                    const size_t ctype = type;
                    std::string currentType = fm::MaterialValueNames::ktypeName[type];

                    static char nameType[256];
                    strcpy(nameType, materialValue->name.c_str());
                    {
                        //ImGui::SameLine();
                        DrawCombo("Type##"+ m->shaderName + materialValue->name, typesMaterial, currentType, &type);
                        ImGui::InputText("TypeName##" + j, nameType, 256);

                    }
                    materialValue->name = std::string(nameType);


                    if(type == fm::ValuesType::VALUE_COLOR)
                    {
                        std::string name = materialValue->name + " "+ fm::MaterialValueNames::ktypeName[type] + "##" + m->shaderName + materialValue->name;
                        fm::Color c =  materialValue->materialValue.getColor();
                        ImGui::ColorEdit3(name.c_str(), &c.r);
                        materialValue->materialValue = c;
                    }
                    else if(type == fm::ValuesType::VALUE_INT)
                    {
                        std::string name = materialValue->name + " "+ fm::MaterialValueNames::ktypeName[type] + "##" + m->shaderName + materialValue->name;

                        int c =  materialValue->materialValue.getInt();
                        ImGui::InputInt(name.c_str(), &c);
                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_FLOAT)
                    {
                        std::string name = materialValue->name + " "+ fm::MaterialValueNames::ktypeName[type] + "##" + m->shaderName + materialValue->name;

                        float c =  materialValue->materialValue.getInt();
                        ImGui::InputFloat(name.c_str(), &c);
                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
                    {
                        std::string name = materialValue->name + " "+ fm::MaterialValueNames::ktypeName[type] + "##" + m->shaderName + materialValue->name;

                        fm::math::vec2 c =  materialValue->materialValue.getVector2();
                        ImGui::InputFloat2(name.c_str(), &c.x);
                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
                    {
                        std::string name = materialValue->name + " "+ fm::MaterialValueNames::ktypeName[type] + "##" + m->shaderName + materialValue->name;

                        fm::math::vec3 c =  materialValue->materialValue.getVector3();
                        ImGui::InputFloat3(name.c_str(), &c.x);
                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
                    {
                        std::string name = materialValue->name + " "+ fm::MaterialValueNames::ktypeName[type] + "##" + m->shaderName + materialValue->name;

                        fm::math::vec4 c =  materialValue->materialValue.getVector4();
                        ImGui::InputFloat4(name.c_str(), &c.x);
                        materialValue->materialValue = c;

                    }


                    j++;

                    if(ctype != type)
                    {
                        materialValue->materialValue.setType((fm::ValuesType)type);

                    }
                    materialValue++;

                    ImGui::Separator();
                }

                if(ImGui::Button("Add"))
                {
                    numberAdded++;
                    m->setValue("NEW", (int)0);
                }

                std::string currentItem = m->shaderName.c_str();
                size_t index = 0;
                DrawCombo("Shader##" + m->GetID(),valuesShader, currentItem, &index);


                if(fm::ResourcesManager::get().Exists<fm::Shader>(currentItem))
                {
                    m->shaderName = currentItem;
                    m->SetFlagHasChanged();
                }

                ImGui::TreePop();
            }
            i++;
        }
    }
}
