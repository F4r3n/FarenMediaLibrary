#include "inspector/transformInspector.hpp"
#include "Components/CTransform.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Transform, fmc::CTransform)

void TransformInspector::init()
{

}

void TransformInspector::draw(bool *)
{
    if(ImGui::CollapsingHeader("Transform"))
    {
        ImGui::PushItemWidth(120);
        ImGui::DragFloat3("Position",&target->transform.position.x, 0.02f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Size", &target->transform.scale.x, 0.02f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Rotation", &target->transform.rotation.x, 0.02f, -FLT_MAX, FLT_MAX);

        ImGui::DragInt("Layer", &target->layer, 1, 0, 99);
        ImGui::PopItemWidth();
    }
}


