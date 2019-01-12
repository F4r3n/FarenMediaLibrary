#include "inspector/meshInspector.hpp"
#include <imgui/imgui.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Mesh, fmc::CMesh)

void MeshInspector::init()
{

}

void MeshInspector::draw(bool *value)
{
    static const char *shapeNames[] = {"Quad", "Circle", "Cube"};

    if(ImGui::CollapsingHeader("Mesh", value))
    {
        ImGui::PushItemWidth(120);
        ImGui::Combo("##Shape", &current, shapeNames, 3);
        ImGui::PopItemWidth();
        target->SetModelType(shapeNames[current]);

    }
}
