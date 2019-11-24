#include "inspector/cameraInspector.hpp"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Camera, fmc::CCamera)

void CameraInspector::init()
{

}

void CameraInspector::draw(bool *value)
{
	std::string id = "##Camera " + std::to_string(target->GetID());

	std::string name = target->GetName() + "##" + std::to_string(target->GetIDEntity());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		float fov = target->GetFOV();
		float farPlane = target->GetFarPlane();
		float nearPlane = target->GetNearPlane();

		bool isDirty = false;

		if (ImGui::DragFloat("FOV", &fov, 0.02f, -180, 180))
		{
			isDirty = true;
			target->SetFov(fov);
		}

		if (ImGui::DragFloat("Near Plane", &nearPlane, 0.02f, 0, FLT_MAX))
		{
			isDirty = true;
			target->SetNearPlane(nearPlane);
		}

		if (ImGui::DragFloat("Far plane", &farPlane, 0.02f, 0, FLT_MAX))
		{
			isDirty = true;
			target->SetFarPlane(farPlane);
		}


		if (isDirty)
		{
			target->UpdateProjectionMatrix();
		}

	}
}


