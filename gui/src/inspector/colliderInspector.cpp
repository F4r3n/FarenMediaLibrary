#include "inspector/colliderInspector.hpp"
#include "Components/CCollider.h"
#include <imgui/imgui.h>
#include <ECS.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Collider, fmc::CCollider)

void ColliderInspector::_Init()
{
}

void ColliderInspector::_DeInit()
{
}

void ColliderInspector::Draw(bool *value, const Entity& e)
{
	std::string id = "##Collider " + std::to_string(_target->GetID());
	static const char *shapeNames[] = { "BOX", "BOX2D", "SPHERE" };
	std::string name = _target->GetName() + "##" + std::to_string(e.id().index());

	if (ImGui::CollapsingHeader(name.c_str(), value))
	{
		fm::math::vec3 currentScale = _target->GetScale();
		int currentShape = (int)_target->GetShape();

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
			_target->SetScale(currentScale);
			_target->SetShape((fmc::CCollider::SHAPE)currentShape);

		}
	}
}

void ColliderInspector::RemoveComponent(const Entity& inEntity)
{
	EntityManager::get().removeComponent<fmc::CCollider>(inEntity.id());
}
