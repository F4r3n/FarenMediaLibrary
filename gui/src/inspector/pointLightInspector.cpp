#include "inspector/pointLightInspector.hpp"
#include "Components/CPointLight.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(PointLight, fmc::CPointLight)

void PointLightInspector::init()
{

}

void PointLightInspector::draw(bool *value)
{
    if(ImGui::CollapsingHeader("PointLight", value))
    {
        ImGui::ColorEdit3("Color##PointLight", &target->color.r);
    }
}
