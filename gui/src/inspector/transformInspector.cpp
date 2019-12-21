#include "inspector/transformInspector.hpp"
#include "Components/CTransform.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Transform, fmc::CTransform)

void TransformInspector::_Init()
{
}

void TransformInspector::_DeInit()
{
}

void TransformInspector::Draw(bool *)
{
	std::string name = _target->GetName() + "##" + std::to_string(_target->GetIDEntity());
	if(ImGui::CollapsingHeader(name.c_str()))
    {
        ImGui::PushItemWidth(120);
        ImGui::DragFloat3("Position",&_target->position.x, 0.02f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat3("Size", &_target->scale.x, 0.02f, -FLT_MAX, FLT_MAX);

		const fm::math::Quaternion q = _target->GetRotation();
		fm::math::vec3 euler = q.GetEulerAngles();
		if (ImGui::DragFloat3("Rotation", &euler.x, 1.0f, -FLT_MAX, FLT_MAX))
		{
			const float pitch = std::max(std::min(euler.x, 89.9f), -89.9f);
			const float y = std::max(std::min(euler.y, 89.9f), -89.9f);
			const float z = std::max(std::min(euler.z, 89.9f), -89.9f);

			_target->SetRotation(fm::math::Quaternion::FromEulerAngles(fm::math::vec3(pitch, y, z)));
		}

        ImGui::PopItemWidth();
    }
}


