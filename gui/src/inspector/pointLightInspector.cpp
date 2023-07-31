#include "inspector/pointLightInspector.hpp"
#include "Components/CPointLight.h"
#include <imgui/imgui.h>
#include "ECS.h"
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(PointLight, fmc::CPointLight)

void PointLightInspector::_Init()
{
}

void PointLightInspector::_DeInit()
{
}

void PointLightInspector::Draw(bool *value, const Entity& inEntity)
{
    if(ImGui::CollapsingHeader("PointLight", value))
    {
        ImGui::ColorEdit3("Color##PointLight", &_target->color.r);
    }
}

void PointLightInspector::RemoveComponent(const Entity& inEntity)
{
	EntityManager::get().removeComponent<fmc::CPointLight>(inEntity.id());
}