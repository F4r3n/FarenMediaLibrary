#include "inspector/colliderInspector.hpp"
#include "Components/CCollider.h"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Collider, fmc::CCollider)

void ColliderInspector::init()
{

}

void ColliderInspector::draw(bool *value)
{
	std::string id = "##Collider " + std::to_string(target->GetID());
	static const char *shapeNames[] = { "BOX", "SPHERE" };
	std::string name = target->GetName() + "##" + std::to_string(target->GetIDEntity());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		fm::math::vec3 currentScale = target->GetScale();
		int currentShape = target->GetShape();

		bool hasChanged = false;
		hasChanged |= ImGui::Combo(std::string("shape" + id).c_str(), &currentShape, shapeNames, 2);
		if (currentShape == fmc::SHAPE::BOX)
		{
			hasChanged |= ImGui::DragFloat3("Scale", &currentScale[0], 0.02f, 0, FLT_MAX);
		}
		else if (currentShape == fmc::SHAPE::SPHERE)
		{
			hasChanged |= ImGui::DragFloat("Radius", &currentScale[0], 0.02f, 0, FLT_MAX);
		}


		if (hasChanged)
		{
			target->SetScale(currentScale);
			target->SetShape((fmc::SHAPE)currentShape);

		}
	}
}


