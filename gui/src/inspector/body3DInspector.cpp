#include "inspector/body3DInspector.hpp"
#include "Components/CBody3D.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Body3D, fmc::CBody3D)

void Body3DInspector::init()
{

}

void Body3DInspector::draw(bool *)
{
	std::string id = "##BODY3D " + std::to_string(target->GetID());
	static const char *shapeNames[] = { "CUBE", "SPHERE" };

	if (ImGui::CollapsingHeader("Body3D"))
	{
		_currentMass = target->GetMass();
		_currentRadius = target->GetRadius();
		_currentScale = target->GetScale();
		_currentShape = target->GetShape();

		ImGui::Combo(std::string("shape" + id).c_str(), &_currentShape, shapeNames, 2);
		if (_currentShape == fmc::SHAPE::BOX)
		{
			ImGui::DragFloat3("Scale", &_currentScale[0], 0.02f, 0, FLT_MAX);
		}
		else if (_currentShape == fmc::SHAPE::SPHERE)
		{
			ImGui::DragFloat("Radius", &_currentRadius, 0.02f, 0, FLT_MAX);
		}
		ImGui::DragFloat("Mass", &_currentMass, 0.001f, 0, FLT_MAX);



		if (_currentShape == fmc::SHAPE::BOX)
		{
			target->SetScale(_currentScale);
		}
		else if (_currentShape == fmc::SHAPE::SPHERE)
		{
			target->SetRadius(_currentRadius);
		}

		target->SetMass(_currentMass);
		
	}
}


