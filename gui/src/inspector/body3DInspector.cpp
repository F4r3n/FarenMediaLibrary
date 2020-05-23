#include "inspector/body3DInspector.hpp"
#include "Components/CBody3D.h"
#include <imgui/imgui.h>
#include <imgui_internal.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Body3D, fmc::CBody3D)

void Body3DInspector::_Init()
{
}

void Body3DInspector::_DeInit()
{
}

void Body3DInspector::Draw(bool *value)
{
	std::string id = "##BODY3D " + std::to_string(_target->GetID());
	static const char *shapeNames[] = { "CUBE", "SPHERE" };
	std::string name = _target->GetName() + "##" + std::to_string(_target->GetIDEntity());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		float currentMass = _target->GetMass();
		bool currentGhost = _target->IsGhost();
		fm::math::vec3 currentGravity = _target->GetGravity();
		if (currentGhost)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		}
		if (ImGui::DragFloat("Mass", &currentMass, 0.001f, 0, FLT_MAX))
		{
			_target->SetMass(currentMass);
		}
		
		if (ImGui::DragFloat3("Gravity", &currentGravity.x, 0.001f, 0, FLT_MAX))
		{
			_target->SetGravity(currentGravity);
		}
		if (currentGhost)
		{
			ImGui::PopItemFlag();
		}
		if (ImGui::Checkbox("Ghost", &currentGhost))
		{
			_target->SetGhost(currentGhost);
		}
		
	}
}


