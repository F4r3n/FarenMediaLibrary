#include "inspector/MaterialInspector.h"
#include <imgui/imgui.h>
#include <Resource/ResourcesManager.h>
#include <Rendering/Shader.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Material, fmc::CMaterial)

void MaterialInspector::draw(bool *value) {

    if(ImGui::CollapsingHeader("Material", value))
    {
        ImGui::ColorEdit3("Color##Material", &target->color.r);


        ImGui::InputText("", shaderName, IM_ARRAYSIZE(shaderName));
        if(fm::ResourcesManager::get().Exists<fm::Shader>(shaderName))
        {
            target->shaderName = std::string(shaderName);
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0,255,0,255),&shaderName[0]);
            target->SetFlagHasChanged();
        }
        else
        {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(255,0,0,255),&shaderName[0]);
        }

    }

}
