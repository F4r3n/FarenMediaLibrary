#include "inspector/meshInspector.hpp"
#include <imgui/imgui.h>
#include "Resource/ResourcesManager.h"
#include "Rendering/Model.hpp"
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Mesh, fmc::CMesh)

void MeshInspector::_Init()
{
}

void MeshInspector::_DeInit()
{
}


void MeshInspector::Draw(bool *value)
{
    static const char *shapeNames[] = {"Quad", "Circle", "Cube"};
	std::string name = _target->GetName() + "##" + std::to_string(_target->GetIDEntity());

    if(ImGui::CollapsingHeader(name.c_str(), value))
    {
		int current = 0;
        ImGui::PushItemWidth(120);
		for (int i = 0; i < 3; ++i)
		{
			if (_target->GetModelType() == shapeNames[i])
			{
				current = i;
				break;
			}
		}
        ImGui::Combo("##Shape", &current, shapeNames, 3);
        ImGui::PopItemWidth();
		_target->SetModelType(shapeNames[current]);
		_target->model = fm::ResourcesManager::get().getResource<fm::Model>(shapeNames[current]);
    }
}
