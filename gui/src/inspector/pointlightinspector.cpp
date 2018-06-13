#include "inspector/pointlightinspector.h"
#include "Components/CPointLight.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(PointLight, fmc::CPointLight)

void PointLightInspector::draw(bool *value)
{
    if(ImGui::CollapsingHeader("PointLight", value))
    {
        ImGui::ColorEdit3("Color", &target->color.r);
    }
}
