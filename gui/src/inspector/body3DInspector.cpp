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
		float currentRestitution = _target->GetRestitution();
		float currentFriction = _target->GetFriction();
		fm::math::vec3 currentAngularFactor = _target->GetAngularFactor();
		fm::math::vec3 currentLinearFactor = _target->GetLinearFactor();


		bool currentGhost = _target->IsGhost();
		fm::math::vec3 currentGravity = _target->GetGravity();
		if (currentGhost)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::DragFloat("Mass", &currentMass, 0.001f, 0, FLT_MAX))
		{
			_target->SetMass(currentMass);
		}
		if (ImGui::DragFloat("Restitution", &currentRestitution, 0.001f, 0, FLT_MAX))
		{
			_target->SetRestitution(currentRestitution);
		}
		if (ImGui::DragFloat("Friction", &currentFriction, 0.001f, 0, FLT_MAX))
		{
			_target->SetFriction(currentFriction);
		}
		if (ImGui::DragFloat3("Gravity", &currentGravity.x, 0.001f, 0, FLT_MAX))
		{
			_target->SetGravity(currentGravity);
		}
		if (ImGui::DragFloat3("Angular Factor", &currentAngularFactor.x, 0.001f, 0, FLT_MAX))
		{
			_target->SetAngularFactor(currentAngularFactor);
		}
		if (ImGui::DragFloat3("Linear factor", &currentLinearFactor.x, 0.001f, 0, FLT_MAX))
		{
			_target->SetLinearFactor(currentLinearFactor);
		}
		if (currentGhost)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		if (ImGui::Checkbox("Ghost", &currentGhost))
		{
			_target->SetGhost(currentGhost);
		}
		
	}
}


