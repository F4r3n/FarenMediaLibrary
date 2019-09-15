#include "inspector/materialInspector.hpp"
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

                    memcpy(nameType, materialValue->name, sizeof(materialValue->name));
                    {
                        DrawCombo("Type##"+ m->shaderName + materialValue->name + std::to_string(j), typesMaterial, currentType, &type);
                        std::string nameTextInput = std::string(nameType) + "##Text"+ m->shaderName + materialValue->name + std::to_string(j);
                        if(ImGui::InputText(nameTextInput.c_str(), nameType, IM_ARRAYSIZE(nameType), ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            strncpy(materialValue->name, nameType, strlen(nameType) );
                        }
                    }

                    std::string name = std::string(materialValue->name) + " "+ fm::MaterialValueNames::ktypeName[type] + "##" + m->shaderName + materialValue->name;
                    name += std::to_string(j);

                    if(type == fm::ValuesType::VALUE_COLOR)
                    {
                        fm::Color c =  materialValue->materialValue.getColor();
                        ImGui::PushID(name.c_str());
                        ImGui::ColorEdit3("##", &c.r);
                        ImGui::PopID();

                        materialValue->materialValue = c;
                    }
                    else if(type == fm::ValuesType::VALUE_INT)
                    {

                        int c =  materialValue->materialValue.getInt();
                        ImGui::PushID(name.c_str());
                        ImGui::InputInt("##", &c);
                        ImGui::PopID();

                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_FLOAT)
                    {

                        float c =  materialValue->materialValue.getInt();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat("##", &c);
                        ImGui::PopID();
                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR2_FLOAT)
                    {

                        fm::math::vec2 c =  materialValue->materialValue.getVector2();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat2("##", &c.x);
                        ImGui::PopID();

                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR3_FLOAT)
                    {

                        fm::math::vec3 c =  materialValue->materialValue.getVector3();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat3("##", &c.x);
                        ImGui::PopID();

                        materialValue->materialValue = c;

                    }
                    else if(type == fm::ValuesType::VALUE_VECTOR4_FLOAT)
                    {

                        fm::math::vec4 c =  materialValue->materialValue.getVector4();
                        ImGui::PushID(name.c_str());
                        ImGui::InputFloat4("##", &c.x);
                        ImGui::PopID();

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
