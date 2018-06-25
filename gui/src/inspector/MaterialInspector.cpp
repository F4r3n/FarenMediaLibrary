#include "inspector/MaterialInspector.h"
#include <imgui/imgui.h>
#include <Resource/ResourcesManager.h>
#include <Rendering/Shader.h>
#include <Core/Debug.h>
#include <Rendering/material.hpp>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Material, fmc::CMaterial)

void MaterialInspector::init()
{
    fm::Debug::get().LogError("InitMaterial");
    std::map<std::string, fm::Resource*> resources = fm::ResourcesManager::get().getAll<fm::Shader>();
    for(auto &r : resources)
    {
        fm::Shader *s = static_cast<fm::Shader*>(r.second);
        values.push_back(s->GetName().c_str());
        lengthBuffer += s->GetName().length();
    }

}

void MaterialInspector::draw(bool *value) {

    if(ImGui::CollapsingHeader("Material", value))
    {
        size_t i = 0;

        for(auto &m :target->GetAllMaterials())
        {
            std::string materialName = m->GetID() + "##Material";

            if (ImGui::TreeNode(materialName.c_str()))
            {
                for(auto &materialValue : m->getValues())
                {
                    fm::ValuesType type =  materialValue.second.getType();
                    if(type == fm::ValuesType::VALUE_COLOR)
                    {
                        std::string name = "Color##" + m->shaderName + materialValue.first;
                        fm::Color c =  materialValue.second.getColor();
                        ImGui::ColorEdit3(name.c_str(), &c.r);
                        materialValue.second = c;

                    }
                }


                const char* item_current_char = m->shaderName.c_str();
                std::string nameCombo = "Shader##" + m->GetID();
                if (ImGui::BeginCombo(nameCombo.c_str(), item_current_char, 0))
                {
                    for (size_t n = 0; n < values.size(); n++)
                    {
                        bool is_selected = (item_current_char == values[n]);
                        if (ImGui::Selectable(values[n], is_selected))
                            item_current_char = values[n];
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }


                if(fm::ResourcesManager::get().Exists<fm::Shader>(item_current_char))
                {
                    m->shaderName = std::string(item_current_char);
                    m->SetFlagHasChanged();
                }

                ImGui::TreePop();
            }
            i++;
        }

    }

}
