#include "inspector/meshInspector.hpp"
#include <imgui/imgui.h>
#include "Resource/ResourcesManager.h"
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Mesh, fmc::CMesh)

void MeshInspector::init()
{

}

void MeshInspector::draw(bool *value)
{
    static const char *shapeNames[] = {"Quad", "Circle", "Cube"};
	std::string name = target->GetName() + "##" + std::to_string(target->GetIDEntity());

    if(ImGui::CollapsingHeader(name.c_str(), value))
    {
        ImGui::PushItemWidth(120);
        ImGui::Combo("##Shape", &current, shapeNames, 3);
        ImGui::PopItemWidth();
		target->SetModelType(shapeNames[current]);
		target->model = fm::ResourcesManager::get().getResource<fm::Model>(shapeNames[current]);
    }
}
