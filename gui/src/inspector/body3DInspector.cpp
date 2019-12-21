#include "inspector/body3DInspector.hpp"
#include "Components/CBody3D.h"
#include <imgui/imgui.h>
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
		_currentMass = _target->GetMass();
		_currentGhost = _target->IsGhost();
		ImGui::DragFloat("Mass", &_currentMass, 0.001f, 0, FLT_MAX);
		ImGui::Checkbox("Ghost", &_currentGhost);
		_target->SetGhost(_currentGhost);
		_target->SetMass(_currentMass);
		
	}
}


