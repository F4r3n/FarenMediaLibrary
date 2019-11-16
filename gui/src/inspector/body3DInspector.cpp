#include "inspector/body3DInspector.hpp"
#include "Components/CBody3D.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Body3D, fmc::CBody3D)

void Body3DInspector::init()
{

}

void Body3DInspector::draw(bool *value)
{
	std::string id = "##BODY3D " + std::to_string(target->GetID());
	static const char *shapeNames[] = { "CUBE", "SPHERE" };
	std::string name = target->GetName() + "##" + std::to_string(target->GetIDEntity());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		_currentMass = target->GetMass();
		_currentGhost = target->IsGhost();
		ImGui::DragFloat("Mass", &_currentMass, 0.001f, 0, FLT_MAX);
		ImGui::Checkbox("Ghost", &_currentGhost);
		target->SetGhost(_currentGhost);
		target->SetMass(_currentMass);
		
	}
}


