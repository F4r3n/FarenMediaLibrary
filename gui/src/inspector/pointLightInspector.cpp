#include "inspector/pointLightInspector.hpp"
#include "Components/CPointLight.h"
#include <imgui/imgui.h>
#include <FML/Core/GameObject.h>
using namespace gui;



void PointLightInspector::Draw(bool *value, std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CPointLight& inTarget = inGameObject->get<fmc::CPointLight>();

    if(ImGui::CollapsingHeader("PointLight", value))
    {
        ImGui::ColorEdit3("Color##PointLight", &inTarget.color.r);
    }
}

void PointLightInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CPointLight>();
}