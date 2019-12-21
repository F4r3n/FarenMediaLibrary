#include "inspector/cameraInspector.hpp"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Camera, fmc::CCamera)

void CameraInspector::_Init()
{
}

void CameraInspector::_DeInit()
{
}

void CameraInspector::Draw(bool *value)
{
	std::string id = "##Camera " + std::to_string(_target->GetID());

	std::string name = _target->GetName() + "##" + std::to_string(_target->GetIDEntity());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		float fov = _target->GetFOV();
		float farPlane = _target->GetFarPlane();
		float nearPlane = _target->GetNearPlane();

		bool isDirty = false;

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


		if (isDirty)
		{
			_target->UpdateProjectionMatrix();
		}

	}
}


