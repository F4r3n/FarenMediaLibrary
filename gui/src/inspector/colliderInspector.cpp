#include "inspector/colliderInspector.hpp"
#include "Components/CCollider.h"
#include <imgui/imgui.h>
#include <FML/Core/GameObject.h>
using namespace gui;


void ColliderInspector::Draw(bool *value, std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CCollider& inTarget = inGameObject->get<fmc::CCollider>();

	std::string id = "##Collider " + std::to_string(inGameObject->GetID());
	static const char *shapeNames[] = { "BOX", "BOX2D", "SPHERE" };
	std::string name = inTarget.GetName() + "##" + std::to_string(inGameObject->GetID());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		fm::math::vec3 currentScale = inTarget.GetScale();
		int currentShape = (int)inTarget.GetShape();

		bool hasChanged = false;
		hasChanged |= ImGui::Combo(std::string("shape" + id).c_str(), &currentShape, shapeNames, (int)fmc::CCollider::SHAPE::LAST);
		if (currentShape == (int)fmc::CCollider::SHAPE::BOX)
		{
			hasChanged |= ImGui::DragFloat3("Scale", &currentScale[0], 0.02f, 0, FLT_MAX);
		}
		else if (currentShape == (int)fmc::CCollider::SHAPE::SPHERE)
		{
			hasChanged |= ImGui::DragFloat("Radius", &currentScale[0], 0.02f, 0, FLT_MAX);
		}


		if (hasChanged)
		{
			inTarget.SetScale(currentScale);
			inTarget.SetShape((fmc::CCollider::SHAPE)currentShape);

		}
	}
}

void ColliderInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CCollider>();
}
