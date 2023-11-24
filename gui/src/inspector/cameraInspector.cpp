#include "inspector/cameraInspector.hpp"
#include <imgui/imgui.h>
#include <imgui_internal.h>
#include <FML/Core/GameObject.h>
#include <FML/Components/CCamera.h>
using namespace gui;


void CameraInspector::Draw(bool *value,  std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CCamera& inTarget = inGameObject->get<fmc::CCamera>();

	std::string id = "##Camera " + std::to_string(inGameObject->GetID());

	std::string name = "Camera##" + std::to_string(inGameObject->GetID());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		float fov = inTarget.GetFOV();
		float farPlane = inTarget.GetFarPlane();
		float nearPlane = inTarget.GetNearPlane();
		bool isOrtho = inTarget.IsOrthographic();

		bool isDirty = false;

		if (ImGui::Checkbox("Orthographic", &isOrtho))
		{
			isDirty = true;
			inTarget.SetOrthoGraphic(isOrtho);
		}

		if (isOrtho)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::DragFloat("FOV", &fov, 0.02f, -180, 180))
		{
			isDirty = true;
			inTarget.SetFov(fov);
		}

		if (ImGui::DragFloat("Near Plane", &nearPlane, 0.02f, 0, FLT_MAX))
		{
			isDirty = true;
			inTarget.SetNearPlane(nearPlane);
		}

		if (ImGui::DragFloat("Far plane", &farPlane, 0.02f, 0, FLT_MAX))
		{
			isDirty = true;
			inTarget.SetFarPlane(farPlane);
		}

		if (isOrtho)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}


		if (isDirty)
		{
			inTarget.UpdateProjectionMatrix();
		}

	}
}

void CameraInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CCamera>();
}

