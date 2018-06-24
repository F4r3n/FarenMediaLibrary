#include "inspector/MaterialInspector.h"
#include <imgui/imgui.h>
#include <Resource/ResourcesManager.h>
#include <Rendering/Shader.h>
#include <Core/Debug.h>
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
            std::string materialName = m.GetID() + "##Material";

            if (ImGui::TreeNode(materialName.c_str()))
            {
                for(auto &materialValue : m.getValues())
                {
                    fm::ValuesType type =  materialValue.second.getType();
                    if(type == fm::ValuesType::VALUE_COLOR)
                    {
                        std::string name = "Color##" + m.shaderName + materialValue.first;
                        fm::Color c =  materialValue.second.getColor();
                        ImGui::ColorEdit3(name.c_str(), &c.r);
                        materialValue.second = c;

                    }
                }
                 ImGui::InputText("name", buf1, 64);

                //ImGui::InputText("", shaderName, 128);
                if(fm::ResourcesManager::get().Exists<fm::Shader>(buf1))
                {
                    m.shaderName = std::string(buf1);
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0,255,0,255),&buf1[0]);
                    m.SetFlagHasChanged();
                }
                else
                {
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(255,0,0,255),&buf1[0]);
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
