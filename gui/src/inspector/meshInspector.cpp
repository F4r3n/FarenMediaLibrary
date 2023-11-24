#include "inspector/meshInspector.hpp"
#include <imgui/imgui.h>
#include "Resource/ResourcesManager.h"
#include "Rendering/Model.hpp"
#include "Core/GameObject.h"
#include "Components/CMesh.h"
using namespace gui;


void MeshInspector::Draw(bool *value, std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CMesh& inTarget = inGameObject->get<fmc::CMesh>();

    static const char *shapeNames[] = {"Quad", "Circle", "Cube"};
	std::string name = inTarget.GetName() + "##" + std::to_string(inGameObject->GetID());

    if(ImGui::CollapsingHeader(name.c_str(), value))
    {
		int current = 0;
        ImGui::PushItemWidth(120);
		for (int i = 0; i < 3; ++i)
		{
			if (inTarget.GetModelPath().GetName(true) == shapeNames[i])
			{
				current = i;
				break;
			}
		}
        ImGui::Combo("##Shape", &current, shapeNames, 3);
        ImGui::PopItemWidth();
		//TODO: Improve to get any meshes
		inTarget.SetModelPath(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, shapeNames[current]));
    }
}

void MeshInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inEntity)
{
	inEntity->remove<fmc::CMesh>();
}