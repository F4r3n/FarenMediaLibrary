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

		fm::math::Quaternion q = target->GetRotation();
		fm::math::vec3 euler = q.GetEulerAngles();
		if (ImGui::DragFloat3("Rotation", &euler.x, 1.0f, -FLT_MAX, FLT_MAX))
		{
			float pitch = std::max(std::min(euler.x, 89.9f), -89.9f);
			target->SetRotation(fm::math::Quaternion::FromEulerAngles(fm::math::vec3(pitch, euler.y, euler.z)));
		}

        ImGui::PopItemWidth();
    }
}


