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

                const char* items[] = { "default","default_light" };
                size_t it = 0;
                for(; it < 2; ++it)
                {
                    if(items[it] == m->shaderName)
                        break;
                }
                int item_current = it == 2 ? it= -1: it;
                ImGui::Combo("Shader", &item_current, items, IM_ARRAYSIZE(items));
                std::cout << item_current << std::endl;
                //ImGui::InputText("", shaderName, 128);
                if(item_current != -1 && fm::ResourcesManager::get().Exists<fm::Shader>(items[item_current]))
                {
                    m->shaderName = std::string(items[item_current]);
                    m->SetFlagHasChanged();
                }

                ImGui::TreePop();
            }
            i++;
        }



        //ImGui::InputText("", shaderName, IM_ARRAYSIZE(shaderName));
        //if(fm::ResourcesManager::get().Exists<fm::Shader>(shaderName))
        //{
        //    target->shaderName = std::string(shaderName);
        //    ImGui::SameLine();
        //    ImGui::TextColored(ImVec4(0,255,0,255),&shaderName[0]);
        //    target->SetFlagHasChanged();
        //}
        //else
        //{
        //    ImGui::SameLine();
        //    ImGui::TextColored(ImVec4(255,0,0,255),&shaderName[0]);
        //}

    }

}
