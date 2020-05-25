#include "inspector/colliderInspector.hpp"
#include "Components/CCollider3D.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Collider3D, fmc::CCollider3D)

void Collider3DInspector::_Init()
{
}

void Collider3DInspector::_DeInit()
{
}

void Collider3DInspector::Draw(bool *value)
{
	std::string id = "##Collider " + std::to_string(_target->GetID());
	static const char *shapeNames[] = { "BOX", "SPHERE" };
	std::string name = _target->GetName() + "##" + std::to_string(_target->GetIDEntity());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		fm::math::vec3 currentScale = _target->GetScale();
		int currentShape = (int)_target->GetShape();

		bool hasChanged = false;
		hasChanged |= ImGui::Combo(std::string("shape" + id).c_str(), &currentShape, shapeNames, 2);
		if (currentShape == (int)fmc::CCollider3D::SHAPE::BOX)
		{
			hasChanged |= ImGui::DragFloat3("Scale", &currentScale[0], 0.02f, 0, FLT_MAX);
		}
		else if (currentShape == (int)fmc::CCollider3D::SHAPE::SPHERE)
		{
			hasChanged |= ImGui::DragFloat("Radius", &currentScale[0], 0.02f, 0, FLT_MAX);
		}


		if (hasChanged)
		{
			_target->SetScale(currentScale);
			_target->SetShape((fmc::CCollider3D::SHAPE)currentShape);

		}
	}
}


