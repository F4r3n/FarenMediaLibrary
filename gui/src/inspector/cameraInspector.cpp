#include "inspector/cameraInspector.hpp"
#include <imgui/imgui.h>
#include <imgui_internal.h>
#include <ECS.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Camera, fmc::CCamera)

void CameraInspector::_Init()
{
}

void CameraInspector::_DeInit()
{
}

void CameraInspector::Draw(bool *value, const Entity& e)
{
	std::string id = "##Camera " + std::to_string(_target->GetID());

	std::string name = _target->GetName() + "##" + std::to_string(e.id().index());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		float fov = _target->GetFOV();
		float farPlane = _target->GetFarPlane();
		float nearPlane = _target->GetNearPlane();
		bool isOrtho = _target->IsOrthographic();

		bool isDirty = false;

		if (ImGui::Checkbox("Orthographic", &isOrtho))
		{
			isDirty = true;
			_target->SetOrthoGraphic(isOrtho);
		}

		if (isOrtho)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		if (ImGui::DragFloat("FOV", &fov, 0.02f, -180, 180))
		{
			isDirty = true;
			_target->SetFov(fov);
		}

		if (ImGui::DragFloat("Near Plane", &nearPlane, 0.02f, 0, FLT_MAX))
		{
			isDirty = true;
			_target->SetNearPlane(nearPlane);
		}

		if (ImGui::DragFloat("Far plane", &farPlane, 0.02f, 0, FLT_MAX))
		{
			isDirty = true;
			_target->SetFarPlane(farPlane);
		}

		if (isOrtho)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}


		if (isDirty)
		{
			_target->UpdateProjectionMatrix();
		}

	}
}

void CameraInspector::RemoveComponent(const Entity& inEntity)
{
	EntityManager::get().removeComponent<fmc::CCamera>(inEntity.id());
}

