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
        ImGui::DragFloat3("Position",&target->position.x, 0.02f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Size", &target->scale.x, 0.02f, -FLT_MAX, FLT_MAX);

		fm::math::Quaternion q = target->rotation;
		fm::math::vec3 euler = q.GetEulerAngles();
        ImGui::DragFloat3("Rotation", &euler.x, 0.02f, -FLT_MAX, FLT_MAX);
		target->rotation.FromEulerAngles(euler);

        ImGui::DragInt("Layer", &target->layer, 1, 0, 99);
        ImGui::PopItemWidth();
    }
}


