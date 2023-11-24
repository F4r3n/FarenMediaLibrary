#include "inspector/transformInspector.hpp"
#include "Components/CTransform.h"
#include <imgui/imgui.h>
#include <FML/Core/GameObject.h>
using namespace gui;



void TransformInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CTransform>();
}

void TransformInspector::Draw(bool *, std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CTransform& inTarget = inGameObject->get<fmc::CTransform>();

	std::string name = inTarget.GetName() + "##" + std::to_string(inGameObject->GetID());
	if(ImGui::CollapsingHeader(name.c_str()))
    {
        ImGui::PushItemWidth(120);
		fm::math::vec3 pos = inTarget.GetPosition();
		fm::math::vec3 scale = inTarget.GetScale();

		if (ImGui::DragFloat3("Position", &pos.x, 0.02f, -FLT_MAX, FLT_MAX))
		{
			inTarget.SetPosition(pos);
		}

		if (ImGui::DragFloat3("Size", &scale.x, 0.02f, -FLT_MAX, FLT_MAX))
		{
			inTarget.SetScale(scale);
		}

		const fm::math::Quaternion q = inTarget.GetRotation();
		fm::math::vec3 euler = q.GetEulerAngles();
		if (ImGui::DragFloat3("Rotation", &euler.x, 1.0f, -FLT_MAX, FLT_MAX))
		{
			const float pitch = std::max(std::min(euler.x, 89.9f), -89.9f);
			const float y = std::max(std::min(euler.y, 89.9f), -89.9f);
			const float z = std::max(std::min(euler.z, 89.9f), -89.9f);

			inTarget.SetRotation(fm::math::Quaternion::FromEulerAngles(fm::math::vec3(pitch, y, z)));
		}

        ImGui::PopItemWidth();
    }
}


