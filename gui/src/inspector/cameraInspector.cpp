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
	
	}
}


