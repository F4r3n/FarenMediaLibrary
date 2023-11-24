#include "inspector/bodyInspector.hpp"
#include <imgui/imgui.h>
#include <imgui_internal.h>
#include <FML/Core/GameObject.h>
#include <FML/Components/CCollider.h>
#include <FML/Components/CBody.h>

using namespace gui;


void BodyInspector::Draw(bool *value, std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CBody& inTarget = inGameObject->get<fmc::CBody>();

	std::string id = "##BODY " + std::to_string(inGameObject->GetID());
	static const char *shapeNames[] = { "CUBE", "SPHERE" };
	std::string name = inTarget.GetName() + "##" + std::to_string(inGameObject->GetID());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		float currentMass = inTarget.GetMass();
		float currentRestitution = inTarget.GetRestitution();
		float currentFriction = inTarget.GetFriction();
		fm::math::vec3 currentAngularFactor = inTarget.GetAngularFactor();
		fm::math::vec3 currentLinearFactor = inTarget.GetLinearFactor();


		bool currentGhost = inTarget.IsGhost();
		fm::math::vec3 currentGravity = inTarget.GetGravity();
		if (currentGhost)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}
		if (ImGui::DragFloat("Mass", &currentMass, 0.001f, 0, FLT_MAX))
		{
			fmc::CCollider* collider = inGameObject->try_get<fmc::CCollider>();
			inTarget.SetMass(currentMass, collider);
		}
		if (ImGui::DragFloat("Restitution", &currentRestitution, 0.001f, 0, FLT_MAX))
		{
			inTarget.SetRestitution(currentRestitution);
		}
		if (ImGui::DragFloat("Friction", &currentFriction, 0.001f, 0, FLT_MAX))
		{
			inTarget.SetFriction(currentFriction);
		}
		if (ImGui::DragFloat3("Gravity", &currentGravity.x, 0.001f, 0, FLT_MAX))
		{
			inTarget.SetGravity(currentGravity);
		}
		if (ImGui::DragFloat3("Angular Factor", &currentAngularFactor.x, 0.001f, 0, FLT_MAX))
		{
			inTarget.SetAngularFactor(currentAngularFactor);
		}
		if (ImGui::DragFloat3("Linear factor", &currentLinearFactor.x, 0.001f, 0, FLT_MAX))
		{
			inTarget.SetLinearFactor(currentLinearFactor);
		}
		if (currentGhost)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		if (ImGui::Checkbox("Ghost", &currentGhost))
		{
			inTarget.SetGhost(currentGhost);
		}
		
	}
}


void BodyInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CBody>();
}